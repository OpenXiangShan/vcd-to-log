//
// Created by ljw on 2021/6/12.
//

#include <gtk/gtk.h>
#include <string>

#include "Trace.h"

typedef struct MyData {
    GtkTextBuffer *buffer;
    Trace* trace;
} MyData;

static void search_text_changed(GtkEntry *entry, MyData *data){
    GtkTextBuffer *buffer = data->buffer;
    Trace* trace = data->trace;
    std::string str;
    for(auto sig : *trace->get_all_signals()){
        str += sig->reference + "\n";
    }
    gtk_text_buffer_set_text(buffer, str.c_str(), str.length());
}




int main(int argc, char **argv){
    GtkBuilder *builder;
    GObject *window;
    GObject *search_bar;
    GObject *search_entry;
    GError *error = NULL;
    GObject *scroll_window;
    GObject *text_view;

    gtk_init(&argc, &argv);


    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "/home/lin/Documents/vcd-to-log/src/builder.xml", &error) == 0){
        g_printerr("error: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    Trace* trace;
    trace = new Trace("/home/lin/Documents/vcd-to-log/examples/LazyModule.vcd");

    window = gtk_builder_get_object(builder, "window");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    text_view = gtk_builder_get_object(builder, "textview");
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "abc", 3);

    search_bar = gtk_builder_get_object(builder, "searchbar");
    gtk_search_bar_set_search_mode(GTK_SEARCH_BAR(search_bar), true);

    search_entry = gtk_builder_get_object(builder, "searchentry");

    MyData data;
    data.trace = trace;
    data.buffer = buffer;
    g_signal_connect(
            search_entry, "search-changed", G_CALLBACK(search_text_changed),
            &data);


    scroll_window = gtk_builder_get_object(builder, "textwindow");
    gtk_container_set_border_width(GTK_CONTAINER(scroll_window), 5);




    gtk_widget_show_all(GTK_WIDGET(scroll_window));


    g_print("111\n");
    gtk_main();
    g_print("111");

    return 0;
}
