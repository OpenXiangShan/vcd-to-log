//
// Created by ljw on 2021/7/24.
//

#include "parse_vcd.h"

static char *buf;
static bool first = true;
static VCDScope *scope = nullptr;

enum Tokens {
    T_VAR, T_END, T_SCOPE, T_UPSCOPE,
    T_COMMENT, T_DATE, T_DUMPALL, T_DUMPOFF, T_DUMPON,
    T_DUMPVARS, T_ENDDEFINITIONS,
    T_DUMPPORTS, T_DUMPPORTSOFF, T_DUMPPORTSON, T_DUMPPORTSALL,
    T_TIMESCALE, T_VERSION, T_VCDCLOSE, T_TIMEZERO,
    NUM_TOKENS, T_EOF, T_STRING
};

static const char *tokens[] = {
        "$var", "$end", "$scope", "$upscope",
        "$comment", "$date", "$dumpall", "$dumpoff", "$dumpon",
        "$dumpvars", "$enddefinitions",
        "$dumpports", "$dumpportsoff", "$dumpportson", "$dumpportsall",
        "$timescale", "$version", "$vcdclose", "$timezero",
};

static const char *time_units[] = {"s", "ms", "us", "ns", "ps"};

static std::pair<int, char *> inline get_token() {
    char *next_word;
    if (first) {
        next_word = strtok((char *) buf, " \n");
        first = false;
    } else {
        next_word = strtok(nullptr, " \n");
    }
//    printf("token: %s\n", next_word);
    if (next_word == nullptr) {
        return std::make_pair(T_EOF, next_word);
    }
    for (int i = 0; i < NUM_TOKENS; i++) {
        if (strcmp(next_word, tokens[i]) == 0) {
            return std::make_pair(i, next_word);
        }
    }
    return std::make_pair(T_STRING, next_word);
}

static void inline skip_until_end() {
    auto token = get_token().first;
    while (token != T_END) {
        token = get_token().first;
    }
}

static std::string inline get_string_until_end() {
    auto[token, str] = get_token();
    std::string s;
    while (token != T_END) {
        if (s.empty()) {
            s += std::string(str);
        } else {
            s += (" " + std::string(str));
        }
        std::tie(token, str) = get_token();
    }
    return s;
}

static void inline parse_timescale(VCDFile *vcd) {
    auto[token, str] = get_token();
    size_t len = strlen(str);
    int time_res = 0;
    VCDTimeUnit time_unit;
    size_t i;
    for (i = 0; i < len; i++) {
        char c = str[i];
        if ('0' <= c && c <= '9') {
            time_res = (c - '0') + time_res * 10;
        } else break;
    }
    assert(len - i <= 2);
    char tu_str[3];
    strncpy(tu_str, str + i, len - i);
    for (int j = 0; j < 5; j++) {
        if (strcmp(tu_str, time_units[j]) == 0) {
            time_unit = static_cast<VCDTimeUnit>(j);
        }
    }
    vcd->time_resolution = time_res;
    vcd->time_units = time_unit;

    get_token(); // must be $end
}

static void inline parse_scope(VCDFile *vcd) {
    VCDScope *child_scope = new VCDScope();
    get_token(); // must be module
    child_scope->type = VCD_SCOPE_MODULE;
    auto name = get_token().second;
    child_scope->name = std::string(name);
    child_scope->parent = scope;
    if (scope != nullptr) {
        scope->children.push_back(child_scope);
    } else {
        vcd->root_scope = child_scope;
    }
    scope = child_scope;
    vcd->add_scope(scope);
    get_token(); // must be end
}

static void inline parse_var(VCDFile *vcd) {
    VCDSignal *sig = new VCDSignal();
    get_token(); // must be 'wire'
    int size = atoi(get_token().second);
    char *hash = get_token().second;
    char *ref = get_token().second;
    skip_until_end();

    sig->hash = hash;
    sig->reference = ref;
    sig->scope = scope;
    sig->size = size;
    sig->type = VCD_VAR_WIRE;
    sig->lindex = size - 1;
    sig->rindex = 0;

    vcd->add_signal(sig);
    scope->signals.push_back(sig);
}

static void inline parse_string(VCDFile *vcd, char *str) {
    size_t len = strlen(str);
    char first_ch = str[0];

    switch (first_ch) {
        case '#': {
            VCDTime t = 0;
            for (int i = 1; i < len; i++) {
                t = t * 10 + (str[i] - '0');
            }
            vcd->add_timestamp(t);
            break;
        }
        case 'b': {
            auto *vec = new VCDBitVector(len - 1);
            for (int i = 1; i < len; i++) {
                (*vec)[i - 1] = static_cast<VCDBit>(str[len - i] - '0');
            }
            char *hash = get_token().second;
            auto *value = new VCDValue(vec);
            auto *tv = new VCDTimedValue;
            tv->value = value;
            tv->time = vcd->get_timestamps()->back();
            vcd->add_signal_value(tv, hash);
            break;
        }
        case 'x':
        case 'z':
        case '0':
        case '1': {
            std::string hash;
            for (int i = 1; i < len; i++) {
                hash += str[i];
            }
            // 'x' and 'z' will be converted to '0'
            auto bit = static_cast<VCDBit>(str[0] == '1');
            auto *value = new VCDValue(bit);
            auto *tv = new VCDTimedValue;
            tv->value = value;
            tv->time = vcd->get_timestamps()->back();
            vcd->add_signal_value(tv, hash);
            break;
        }
        default:
            printf("unknown string type token: %s\n", str);
            assert(false);
    }
}

static void handle_error(const char *msg) {
    perror(msg);
    exit(-1);
}

static char *map_file(int fd, size_t &length) {
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        handle_error("fstat");
    }
    length = sb.st_size + 1;
    char *addr = static_cast<char *>(mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0u));
    if (addr == MAP_FAILED) {
        handle_error("mmap");
    }
    addr[length - 1] = '\0';
    return addr;
}

VCDFile *parse_vcd(const char *fpath) {
    int fd = open(fpath, O_RDONLY);
    if (fd < 3) {
        handle_error("open");
    }
    size_t length;
    buf = (char *) map_file(fd, length);
    auto *vcd = new VCDFile();
    auto[token, str] = get_token();
    while (token != T_EOF) {
        switch (token) {
            case T_VERSION:
                vcd->version = get_string_until_end();
                break;
            case T_DATE:
                vcd->date = get_string_until_end();
                break;
            case T_COMMENT:
                skip_until_end();
                break;
            case T_TIMESCALE:
                parse_timescale(vcd);
                break;
            case T_SCOPE:
                parse_scope(vcd);
                break;
            case T_VAR:
                parse_var(vcd);
                break;
            case T_UPSCOPE:
                scope = scope->parent;
                break;
            case T_DUMPVARS:
//                skip_until_end();
                break;
            case T_STRING:
                parse_string(vcd, str);
                break;

        }
        std::tie(token, str) = get_token();
    }
    close(fd);
    munmap(buf, length);
    return vcd;
}