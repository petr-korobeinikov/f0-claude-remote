#include "view.h"

#include <f0_claude_remote_icons.h>

static void view_draw_white_str(Canvas* canvas, int32_t x, int32_t y, const char* str) {
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_str(canvas, x, y, str);
    canvas_set_color(canvas, ColorBlack);
}

static void view_draw_white_icon(Canvas* canvas, int32_t x, int32_t y, const Icon* icon) {
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_icon(canvas, x, y, icon);
    canvas_set_color(canvas, ColorBlack);
}

static void view_draw_split(Canvas* canvas, int32_t x, int32_t y, size_t width, int32_t hold_x) {
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x, y, width, 11);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, x, y, width, 11, 2);
    canvas_draw_rbox(canvas, hold_x, y, x + width - hold_x, 11, 2);
    canvas_draw_box(canvas, hold_x, y, 2, 11);
}

static void view_draw_status_row(Canvas* canvas) {
    canvas_draw_icon(canvas, 2, 1, &I_bluetooth);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 12, 9, "Connected");
    canvas_draw_line(canvas, 0, 12, 127, 12);
}

static void view_draw_legend(Canvas* canvas) {
    canvas_set_font(canvas, FontSecondary);
    view_draw_split(canvas, 74, 0, 53, 97);
    canvas_draw_str(canvas, 77, 9, "TAP");
    view_draw_white_str(canvas, 100, 9, "HOLD");
}

static void view_draw_ring(Canvas* canvas) {
    canvas_draw_circle(canvas, 60, 38, 20);
    view_draw_split(canvas, 54, 14, 35, 66);
    canvas_draw_icon(canvas, 57, 17, &I_up);
    view_draw_white_str(canvas, 69, 22, "high");
    view_draw_split(canvas, 54, 52, 36, 66);
    canvas_draw_icon(canvas, 57, 55, &I_down);
    view_draw_white_str(canvas, 69, 61, "max");
}

static void view_draw_ok_button(Canvas* canvas) {
    canvas_draw_rframe(canvas, 45, 28, 30, 21, 2);
    canvas_draw_rbox(canvas, 45, 38, 30, 11, 2);
    canvas_draw_box(canvas, 45, 38, 30, 2);
    canvas_draw_str(canvas, 48, 37, "Enter");
    view_draw_white_str(canvas, 49, 47, "voice");
}

static void view_draw_side_actions(Canvas* canvas) {
    canvas_draw_rbox(canvas, 3, 33, 34, 11, 2);
    view_draw_white_str(canvas, 6, 42, "clear");
    view_draw_white_icon(canvas, 30, 35, &I_left);
    canvas_draw_rframe(canvas, 84, 33, 28, 11, 2);
    canvas_draw_icon(canvas, 87, 35, &I_right);
    canvas_draw_str(canvas, 94, 42, "Esc");
    canvas_draw_rbox(canvas, 3, 52, 33, 11, 2);
    view_draw_white_icon(canvas, 6, 54, &I_back);
    view_draw_white_str(canvas, 17, 61, "exit");
}

static void view_draw_flash(Canvas* canvas, Action flash) {
    canvas_set_color(canvas, ColorXOR);
    switch(flash) {
    case ActionEffortHigh:
        canvas_draw_box(canvas, 67, 15, 21, 9);
        break;
    case ActionEffortMax:
        canvas_draw_box(canvas, 67, 53, 22, 9);
        break;
    case ActionClear:
        canvas_draw_box(canvas, 4, 34, 32, 9);
        break;
    case ActionEsc:
        canvas_draw_box(canvas, 85, 34, 26, 9);
        break;
    default:
        break;
    }
    canvas_set_color(canvas, ColorBlack);
}

static void view_draw_voice(Canvas* canvas) {
    canvas_draw_box(canvas, 0, 0, 128, 64);
    canvas_set_color(canvas, ColorWhite);
    view_draw_status_row(canvas);
    canvas_draw_icon(canvas, 22, 27, &I_microphone_large);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 48, 37, "Listening");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 48, 48, "Release OK to stop");
    canvas_set_color(canvas, ColorBlack);
}

void view_draw(Canvas* canvas, bool voice_on, Action flash) {
    if(voice_on) {
        view_draw_voice(canvas);
        return;
    }

    view_draw_status_row(canvas);
    view_draw_legend(canvas);
    view_draw_ring(canvas);
    view_draw_ok_button(canvas);
    view_draw_side_actions(canvas);
    view_draw_flash(canvas, flash);
}
