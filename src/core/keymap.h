#pragma once

#include <stdbool.h>

#include "action.h"

typedef enum {
    KeymapKeyUp,
    KeymapKeyDown,
    KeymapKeyLeft,
    KeymapKeyRight,
    KeymapKeyOk,
    KeymapKeyBack,
    KeymapKeyMAX,
} KeymapKey;

typedef enum {
    KeymapEventPress,
    KeymapEventShort,
    KeymapEventLong,
    KeymapEventRepeat,
    KeymapEventRelease,
    KeymapEventMAX,
} KeymapEvent;

typedef struct Keymap Keymap;

Keymap* keymap_alloc(void);

void keymap_free(Keymap* keymap);

Action keymap_handle(Keymap* keymap, KeymapKey key, KeymapEvent event);

bool keymap_is_voice_on(const Keymap* keymap);
