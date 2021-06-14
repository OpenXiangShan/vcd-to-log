//
// Created by ljw on 2021/6/12.
//

#include <gtk/gtk.h>
#include <string>

#include "Trace.h"

typedef struct SelHeirData {
    GtkStack *sig_stack;
    GtkEntryCompletion *completion;
} SelHeirData;

typedef struct MatchSelData {
    GtkTextBuffer *text_buffer;
    GtkSearchBar *search_bar;
    GtkWindow *window;
    GtkWidget *focus;
} MatchSelData;

typedef struct InvokeSearchData {
    GtkSearchBar *search_bar;
    GtkWindow *window;
    GtkWidget *focus;
} InvokeSearchData;

typedef struct StopSearchCbData {
    GtkSearchBar *search_bar;
    GtkWindow *window;
    GtkWidget *focus;
} StopSearchCbData;

void append_tree_iter(Trace *trace, GtkTreeStore *model, GtkTreeIter *iter, VCDScope *scope, GtkStack *stack) {
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

GtkTreeModel *create_model(Trace *trace, GtkStack *stack){
    GtkTreeStore *model;
    VCDScope *scope;
    model = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    scope = trace->get_root_scope();
    scope = scope->children[0];
    append_tree_iter(trace, model, NULL, scope, stack);
    return GTK_TREE_MODEL(model);
}

gboolean select_heir(
        GtkTreeSelection *selection,
        GtkTreeModel *model,
        GtkTreePath *path,
        gboolean path_current_selected,
        gpointer data
        ){

    SelHeirData *m_data;
    GtkStack *sig_stack;
    GtkEntryCompletion *completion;

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
            m_data = reinterpret_cast<SelHeirData *>(data);
            sig_stack = m_data->sig_stack;
            completion = m_data->completion;
            assert(completion);
            g_print("will select %s[%s]\n", name, full_name);
            // change visible child
            gtk_stack_set_visible_child_name(sig_stack, full_name);
            // change search entry completion
            sw = reinterpret_cast<GtkScrolledWindow *>(gtk_stack_get_visible_child(sig_stack));
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
    MatchSelData *m_data;
    GtkTextBuffer *text_buffer;
    GtkTextIter text_iter;
    GtkSearchBar *search_bar;
    gchar *name;
    gchar *full_name;
    GtkEntry *entry;
    assert(model);
    g_print("match\n");
    gtk_tree_model_get(model, iter, 0, &name, 1, &full_name, -1);


    m_data = reinterpret_cast<MatchSelData *>(data);
    text_buffer = m_data->text_buffer;
    search_bar = m_data->search_bar;

//    gtk_search_bar_set_search_mode(search_bar, false);
    entry = GTK_ENTRY(gtk_entry_completion_get_entry(completion));
    gtk_entry_set_text(entry, "");

    gtk_text_buffer_insert_at_cursor(text_buffer, full_name, strlen(full_name));
//    gtk_text_buffer_get_end_iter(text_buffer, &text_iter);
//    gtk_text_buffer_place_cursor(text_buffer, &text_iter);

    gtk_window_set_focus(GTK_WINDOW(m_data->window), GTK_WIDGET(m_data->focus));
    g_free(full_name);
    return true;
}

static void invoke_search_cb(GtkWidget *widget, gpointer data){
    g_print("invoke search\n");
    InvokeSearchData *m_data = reinterpret_cast<InvokeSearchData *>(data);
    gtk_search_bar_set_search_mode(m_data->search_bar, true);
    gtk_window_set_focus(m_data->window, m_data->focus);
}

static void stop_search_cb(GtkSearchEntry *entry, gpointer data){
    gtk_entry_set_text(GTK_ENTRY(entry), "");
    StopSearchCbData *m_data = reinterpret_cast<StopSearchCbData *>(data);
    gtk_search_bar_set_search_mode(m_data->search_bar, true);
    gtk_window_set_focus(m_data->window, m_data->focus);
}


int main(int argc, char **argv){
    GtkBuilder *builder;

    GObject *window;

    GObject *menu_bar;
    GObject *search_bar;
    GObject *search_entry;

    GtkEntryCompletion *completion;
    GtkTreeModel *heir_model;
    GtkWidget *heir_view;
    GtkCellRenderer *renderer;
    GObject *sig_stack;
    GObject *heir_window;

    GObject *text_window;
    GObject *text_view;
    GtkTextBuffer *buffer;

    GError *error = NULL;

    Trace* trace;
    trace = new Trace("/home/lin/Documents/vcd-to-log/examples/LazyModule.vcd");
    assert(trace);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "/home/lin/Documents/vcd-to-log/src/builder.xml", &error) == 0){
        g_printerr("error: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // find objects
    window = gtk_builder_get_object(builder, "window");

    menu_bar = gtk_builder_get_object(builder, "menu_bar");
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
        char *fname;
        fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g_print("fname: %s\n", fname);
    }
    gtk_widget_destroy(dialog);

    search_bar = gtk_builder_get_object(builder, "searchbar");
    search_entry = gtk_builder_get_object(builder, "search_entry");

    sig_stack = gtk_builder_get_object(builder, "sig_stack");
    heir_window = gtk_builder_get_object(builder, "heir_window");

    text_window = gtk_builder_get_object(builder, "text_window");
    text_view = gtk_builder_get_object(builder, "text_view");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // init text window
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "abc", 3);

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

    gtk_stack_set_visible_child_name(GTK_STACK(sig_stack), "LazyModule");

    completion = gtk_entry_completion_new();
    gtk_entry_set_completion(GTK_ENTRY(search_entry), completion);
    gtk_entry_completion_set_model(completion, NULL);
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_completion_set_minimum_key_length(completion, 1);
    MatchSelData match_data = {
            .text_buffer = buffer,
            .search_bar = GTK_SEARCH_BAR(search_bar),
            .window = GTK_WINDOW(window),
            .focus = GTK_WIDGET(text_view)
    };
    g_signal_connect(
            completion,
            "match-selected",
            G_CALLBACK(match_selected_cb),
            &match_data);

    SelHeirData sel_data = {.sig_stack = GTK_STACK(sig_stack), .completion = completion};
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(heir_view));
    gtk_tree_selection_set_select_function(
            selection,
            select_heir,
            &sel_data,
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

    InvokeSearchData inv_data = {
            .search_bar = GTK_SEARCH_BAR(search_bar),
            .window = GTK_WINDOW(window),
            .focus = GTK_WIDGET(search_entry)
    };

    g_signal_connect(
            GTK_WIDGET(text_view),
            "invoke-search",
            G_CALLBACK(invoke_search_cb),
            &inv_data);

    StopSearchCbData stop_data = {.search_bar = GTK_SEARCH_BAR(search_bar), .window = GTK_WINDOW(window), .focus = GTK_WIDGET(text_view)};
    g_signal_connect(
            search_entry,
            "stop-search",
            G_CALLBACK(stop_search_cb),
            &stop_data
            );

    gtk_widget_show_all(GTK_WIDGET(text_window));
    gtk_widget_show_all(GTK_WIDGET(heir_window));
    gtk_widget_show_all(GTK_WIDGET(sig_stack));
    gtk_window_set_focus(GTK_WINDOW(window), GTK_WIDGET(text_view));
    gtk_main();

    return 0;
}
