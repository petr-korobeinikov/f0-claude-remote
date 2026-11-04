#pragma once

#include <furi.h>

#include "../core/action.h"

typedef struct Screen Screen;

Screen* screen_alloc(FuriEventLoop* event_loop, FuriMessageQueue* input_queue);

void screen_free(Screen* screen);

void screen_set_voice(Screen* screen, bool voice_on);

void screen_flash(Screen* screen, Action action, uint32_t duration_ms);
