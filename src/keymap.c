#include "keymap.h"

#include <stdlib.h>

struct Keymap {
    bool voice_on;
};

/* Esc acts on press: past the long press a slow one would otherwise do nothing. */
static const Action keymap_actions[KeymapKeyMAX][KeymapEventMAX] = {
    [KeymapKeyUp] = {[KeymapEventShort] = ActionArrowUp, [KeymapEventLong] = ActionEffortHigh},
    [KeymapKeyDown] = {[KeymapEventShort] = ActionArrowDown, [KeymapEventLong] = ActionEffortMax},
    [KeymapKeyLeft] = {[KeymapEventLong] = ActionClear},
    [KeymapKeyRight] = {[KeymapEventPress] = ActionEsc},
    [KeymapKeyOk] = {[KeymapEventShort] = ActionEnter, [KeymapEventLong] = ActionVoiceStart},
    [KeymapKeyBack] = {[KeymapEventLong] = ActionExit},
};

Keymap* keymap_alloc(void) {
    Keymap* keymap = malloc(sizeof(Keymap));
    keymap->voice_on = false;
    return keymap;
}

void keymap_free(Keymap* keymap) {
    free(keymap);
}

Action keymap_handle(Keymap* keymap, KeymapKey key, KeymapEvent event) {
    if(keymap->voice_on) {
        if(key != KeymapKeyOk || event != KeymapEventRelease) {
            return ActionNone;
        }
        keymap->voice_on = false;
        return ActionVoiceStop;
    }

    Action action = keymap_actions[key][event];
    keymap->voice_on = action == ActionVoiceStart;
    return action;
}

bool keymap_is_voice_on(const Keymap* keymap) {
    return keymap->voice_on;
}
