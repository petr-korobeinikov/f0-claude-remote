#include <furi.h>
#include <gui/gui.h>

#include "view.h"

static void app_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    view_draw(canvas);
}

/* Runs on the GUI thread, and event_loop.h binds its functions to the loop's own thread. */
static void app_input_callback(InputEvent* event, void* context) {
    FuriMessageQueue* queue = context;
    furi_message_queue_put(queue, event, 0);
}

static void app_queue_callback(FuriEventLoopObject* object, void* context) {
    FuriEventLoop* event_loop = context;
    InputEvent event;
    furi_check(furi_message_queue_get(object, &event, 0) == FuriStatusOk);

    if(event.type == InputTypeLong && event.key == InputKeyBack) {
        furi_event_loop_stop(event_loop);
    }
}

int32_t app_main(void* argument) {
    UNUSED(argument);

    FuriEventLoop* event_loop = furi_event_loop_alloc();
    FuriMessageQueue* queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    furi_event_loop_subscribe_message_queue(
        event_loop, queue, FuriEventLoopEventIn, app_queue_callback, event_loop);

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, app_draw_callback, NULL);
    view_port_input_callback_set(view_port, app_input_callback, queue);
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    furi_event_loop_run(event_loop);

    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);

    furi_event_loop_unsubscribe(event_loop, queue);
    furi_message_queue_free(queue);
    furi_event_loop_free(event_loop);

    return 0;
}
