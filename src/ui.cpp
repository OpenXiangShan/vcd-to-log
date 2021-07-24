//
// Created by ljw on 2021/6/12.
//

#include <gtk/gtk.h>
#include <string>

#include "Trace.h"

static GtkBuilder *builder;
static GObject *window;
static GObject *menu_bar;

static GObject *search_bar;
static GObject *search_entry;

static GtkEntryCompletion *completion;
static GtkTreeModel *heir_model;
static GtkWidget *heir_view;
static GtkCellRenderer *renderer;
static GObject *sig_stack;
static GObject *heir_window;

static GObject *text_window;
static GObject *text_view;
static GtkTextBuffer *buffer;

static GError *error = NULL;

static GFile *xiang_file;

static void append_tree_iter(Trace *trace, GtkTreeStore *model, GtkTreeIter *iter, VCDScope *scope, GtkStack *stack) {
    static int cnt = 0;
    std::string full_name = trace->get_full_name(scope);

    // create a new view in stack
    if(!full_name.empty()){
        GtkListStore *lst;
        GtkTreeIter s_it;
        GtkCellRenderer *renderer;

        renderer = gtk_cell_renderer_text_new();
        lst = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
        for(auto sig : scope->signals){
            gtk_list_store_append(lst, &s_it);
            gtk_list_store_set(
                    lst, &s_it,
                    0, sig->reference.c_str(),
                    1, (trace->get_full_name(scope) + "." + sig->reference).c_str(),
                    -1);
        }
        GtkWidget *lview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(lst));
        gtk_tree_view_insert_column_with_attributes(
                GTK_TREE_VIEW(lview),
                -1,
                "Signals",
                renderer,
                "text", 0, NULL
        );
        GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(sw), lview);
        gtk_stack_add_named(stack, sw, full_name.c_str());
        cnt ++;
    }

    for(auto child : scope->children){
        GtkTreeIter it;
        gtk_tree_store_append(model, &it, iter);
        gtk_tree_store_set(model, &it, 0, child->name.c_str(), 1, trace->get_full_name(child).c_str(), -1);
        append_tree_iter(trace, model, &it, child, stack);
    }
}

static GtkTreeModel *create_model(Trace *trace, GtkStack *stack){
    GtkTreeStore *model;
    VCDScope *scope;
    model = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    scope = trace->get_root_scope();
    scope = scope->children[0];
    append_tree_iter(trace, model, NULL, scope, stack);
    return GTK_TREE_MODEL(model);
}

static gboolean select_heir(
        GtkTreeSelection *selection,
        GtkTreeModel *model,
        GtkTreePath *path,
        gboolean path_current_selected,
        gpointer data
        ){

    GtkTreeIter iter;
    gchar *name;
    gchar *full_name;
    GtkScrolledWindow *sw;
    GtkTreeView *sig_view;
    GtkTreeModel *sig_model;

    if(gtk_tree_model_get_iter(model, &iter, path)){
        gtk_tree_model_get(model, &iter, 0, &name, 1, &full_name, -1);
        if(path_current_selected){
            g_print("will unselect %s[%s]\n", name, full_name);
        } else {
            g_print("will select %s[%s]\n", name, full_name);
            // change visible child
            gtk_stack_set_visible_child_name(GTK_STACK(sig_stack), full_name);
            // change search entry completion
            sw = reinterpret_cast<GtkScrolledWindow *>(gtk_stack_get_visible_child(GTK_STACK(sig_stack)));
            GList *list = gtk_container_get_children(GTK_CONTAINER(sw));
            sig_view = GTK_TREE_VIEW(list->data);
            sig_model = gtk_tree_view_get_model(sig_view);
            gtk_entry_completion_set_model(completion, sig_model);
            gtk_entry_completion_set_text_column(completion, 0);
            gtk_entry_completion_set_minimum_key_length(completion, 1);
        }
        g_free(name);
        g_free(full_name);
    }
    return true;
}

static gboolean match_selected_cb(
        GtkEntryCompletion *completion,
        GtkTreeModel *model,
        GtkTreeIter *iter,
        gpointer data
        ) {
    gchar *name;
    gchar *full_name;
    GtkEntry *entry;
    assert(model);
    g_print("match\n");
    gtk_tree_model_get(model, iter, 0, &name, 1, &full_name, -1);

    entry = GTK_ENTRY(gtk_entry_completion_get_entry(completion));
    gtk_entry_set_text(entry, "");

    gtk_text_buffer_insert_at_cursor(buffer, full_name, strlen(full_name));

    gtk_window_set_focus(GTK_WINDOW(window), GTK_WIDGET(text_view));
    g_free(full_name);
    return true;
}

static void invoke_search_cb(GtkWidget *widget, gpointer data){
    g_print("invoke search\n");
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(search_bar), true);
    gtk_window_set_focus(GTK_WINDOW(window), GTK_WIDGET(search_entry));
}

static void stop_search_cb(GtkSearchEntry *entry, gpointer data){
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(search_bar), true);
    gtk_window_set_focus(GTK_WINDOW(window), GTK_WIDGET(text_view));
}

static void open_file(GtkMenuItem *item, gpointer data){
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new(
            "Open",
            GTK_WINDOW(window),
            GTK_FILE_CHOOSER_ACTION_OPEN,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Open",
            GTK_RESPONSE_ACCEPT,
            NULL
    );
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        gchar *basename, *contents;
        gsize len;
        xiang_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
        basename = g_file_get_basename(xiang_file);
        if(g_file_load_contents(xiang_file, NULL, &contents, &len, NULL, NULL)){
            gtk_text_buffer_set_text(buffer, contents, len);
            g_free(contents);
        }
        g_free(basename);
    }
    gtk_widget_destroy(dialog);
}

static void save_file(GtkMenuItem *item, gpointer data){
    if(xiang_file){
        gchar *contents;
        GtkTextIter start, end;
        gchar *path = g_file_get_path(xiang_file);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        contents = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
        g_file_set_contents(path, contents, -1, NULL);
        g_free(path);
    }
}

static void save_file_as(GtkMenuItem *item, gpointer data){
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    dialog = gtk_file_chooser_dialog_new(
            "Save File",
            GTK_WINDOW(window),
            GTK_FILE_CHOOSER_ACTION_SAVE,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Save",
            GTK_RESPONSE_ACCEPT,
            NULL
    );
    chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    if(xiang_file){
        gchar *basename = g_file_get_basename(xiang_file);
        gtk_file_chooser_set_filename(chooser, basename);
        g_free(basename);
    }
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if(res == GTK_RESPONSE_ACCEPT){
        gchar *file_name, *contents;
        GtkTextIter start, end;
        file_name = gtk_file_chooser_get_filename(chooser);
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        contents = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
        g_file_set_contents(file_name, contents, -1, NULL);
    }
    gtk_widget_destroy(dialog);
}


int main(int argc, char **argv){

    if(argc != 2){
        g_print("usage: %s <path-to-vcd-file>\n", argv[0]);
        return -1;
    }

    char *vcd_path = argv[1];

    char *builder_path = "../../src/builder.xml";

    Trace* trace;
    trace = new Trace(vcd_path);
    assert(trace);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, builder_path, &error) == 0){
        g_printerr("error: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // find objects
    window = gtk_builder_get_object(builder, "window");

    menu_bar = gtk_builder_get_object(builder, "menu_bar");

    GtkMenuItem *open_file_button;
    open_file_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "open_file"));
    GtkMenuItem *save_file_button;
    save_file_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "save_file"));
    GtkMenuItem *save_as_button;
    save_as_button = GTK_MENU_ITEM(gtk_builder_get_object(builder, "save_as"));

    g_signal_connect(open_file_button, "activate", G_CALLBACK(open_file), NULL);
    g_signal_connect(save_file_button, "activate", G_CALLBACK(save_file), NULL);
    g_signal_connect(save_as_button, "activate", G_CALLBACK(save_file_as), NULL);


    search_bar = gtk_builder_get_object(builder, "searchbar");
    search_entry = gtk_builder_get_object(builder, "search_entry");

    sig_stack = gtk_builder_get_object(builder, "sig_stack");
    heir_window = gtk_builder_get_object(builder, "heir_window");

    text_window = gtk_builder_get_object(builder, "text_window");
    text_view = gtk_builder_get_object(builder, "text_view");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // init text window
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // init search bar
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(search_bar), true);

    // init tree window
    heir_model = create_model(trace, GTK_STACK(sig_stack));
    heir_view = gtk_tree_view_new_with_model(heir_model);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(
            GTK_TREE_VIEW(heir_view),
            -1,
            "Hierarchy",
            renderer,
            "text", 0, NULL
            );
    gtk_container_add(GTK_CONTAINER(heir_window), heir_view);

    completion = gtk_entry_completion_new();
    gtk_entry_set_completion(GTK_ENTRY(search_entry), completion);
    gtk_entry_completion_set_model(completion, NULL);
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_completion_set_minimum_key_length(completion, 1);

    g_signal_connect(
            completion,
            "match-selected",
            G_CALLBACK(match_selected_cb),
            NULL);

    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(heir_view));
    gtk_tree_selection_set_select_function(
            selection,
            select_heir,
            NULL,
            NULL);

    g_signal_new(
            "invoke-search",
            G_TYPE_OBJECT,
            G_SIGNAL_ACTION,
            NULL,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__VOID,
            G_TYPE_NONE,
            0,
            NULL
    );

    GtkAccelGroup *ag = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), ag);
    gtk_widget_add_accelerator(
            GTK_WIDGET(text_view),
            "invoke-search",
            ag,
            GDK_KEY_f,
            GDK_CONTROL_MASK,
            GTK_ACCEL_VISIBLE
            );

    g_signal_connect(
            GTK_WIDGET(text_view),
            "invoke-search",
            G_CALLBACK(invoke_search_cb),
            NULL);

    g_signal_connect(
            search_entry,
            "stop-search",
            G_CALLBACK(stop_search_cb),
            NULL
            );

    gtk_widget_show_all(GTK_WIDGET(text_window));
    gtk_widget_show_all(GTK_WIDGET(heir_window));
    gtk_widget_show_all(GTK_WIDGET(sig_stack));
    gtk_window_set_focus(GTK_WINDOW(window), GTK_WIDGET(text_view));
    gtk_main();

    return 0;
}
