#include <furi.h>
#include <notification/notification_messages.h>

#include "feedback.h"
#include "keymap_input.h"
#include "screen.h"

typedef struct {
    FuriEventLoop* event_loop;
    FuriMessageQueue* input_queue;
    NotificationApp* notification;
    Keymap* keymap;
    Screen* screen;
} App;

static void app_perform(App* app, Action action) {
    if(feedback_vibrates(action)) {
        notification_message(app->notification, &sequence_single_vibro);
    }
    if(feedback_flashes(action)) {
        screen_flash(app->screen, action);
    }
    screen_set_voice(app->screen, keymap_is_voice_on(app->keymap));
    if(action == ActionExit) {
        furi_event_loop_stop(app->event_loop);
    }
}

static void app_queue_callback(FuriEventLoopObject* object, void* context) {
    App* app = context;
    InputEvent event;
    furi_check(furi_message_queue_get(object, &event, 0) == FuriStatusOk);

    Action action = keymap_input_handle(app->keymap, &event);
    if(action != ActionNone) {
        app_perform(app, action);
    }
}

static App* app_alloc(void) {
    App* app = malloc(sizeof(App));
    app->event_loop = furi_event_loop_alloc();
    app->input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    furi_event_loop_subscribe_message_queue(
        app->event_loop, app->input_queue, FuriEventLoopEventIn, app_queue_callback, app);
    app->notification = furi_record_open(RECORD_NOTIFICATION);
    app->keymap = keymap_alloc();
    app->screen = screen_alloc(app->event_loop, app->input_queue);
    return app;
}

static void app_free(App* app) {
    screen_free(app->screen);
    keymap_free(app->keymap);
    furi_record_close(RECORD_NOTIFICATION);
    furi_event_loop_unsubscribe(app->event_loop, app->input_queue);
    furi_message_queue_free(app->input_queue);
    furi_event_loop_free(app->event_loop);
    free(app);
}

int32_t app_main(void* argument) {
    UNUSED(argument);

    App* app = app_alloc();
    furi_event_loop_run(app->event_loop);
    app_free(app);

    return 0;
}
