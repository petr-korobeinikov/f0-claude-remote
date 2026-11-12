#include "keymap.h"

#include <stdlib.h>
#include <string.h>

/* The firmware's fast advertising lasts 60 s (gap.c:430-435), and stopping sooner than the 30 s
 * Bluetooth gives pairing, plus the walk to the Mac's settings, cuts the link just paired. */
static const uint32_t keymap_link_wait_ms = 60000;

struct Keymap {
    Mode mode;
    bool pressed[KeymapKeyMAX];
    Link link;
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

static const Link keymap_other_links[LinkMAX] = {
    [LinkBluetooth] = LinkUsb,
    [LinkUsb] = LinkBluetooth,
};

static const Action keymap_use_actions[LinkMAX] = {
    [LinkBluetooth] = ActionUseBluetooth,
    [LinkUsb] = ActionUseUsb,
};

static void keymap_forget_presses(Keymap* keymap) {
    memset(keymap->pressed, 0, sizeof(keymap->pressed));
}

static void keymap_set_screen(Keymap* keymap, Mode mode, Link link) {
    if(mode != keymap->mode || link != keymap->link) {
        keymap->mode = mode;
        keymap->link = link;
        keymap_forget_presses(keymap);
    }
}

static void keymap_set_mode(Keymap* keymap, Mode mode) {
    keymap_set_screen(keymap, mode, keymap->link);
}

Keymap* keymap_alloc(void) {
    Keymap* keymap = malloc(sizeof(Keymap));
    keymap->mode = ModeNoLink;
    keymap_forget_presses(keymap);
    keymap->link = LinkBluetooth;
    return keymap;
}

void keymap_free(Keymap* keymap) {
    free(keymap);
}

Action keymap_handle(Keymap* keymap, KeymapKey key, KeymapEvent event) {
    bool ok_released = key == KeymapKeyOk && event == KeymapEventRelease;
    bool gesture = event == KeymapEventShort || event == KeymapEventLong;
    Action action = keymap_actions[key][event];

    if(event == KeymapEventPress) {
        keymap->pressed[key] = true;
    } else if(event == KeymapEventRelease) {
        keymap->pressed[key] = false;
    }

    /* The long press fires 300 ms after the press and the tap comes with the release, so a gesture
     * begun on another screen would act here on a press meant for that one: starting over, then
     * Enter or voice on the main screen. Back's hold only leaves, which every screen but voice
     * allows. */
    if(gesture && !keymap->pressed[key] && action != ActionExit) {
        return ActionNone;
    }

    switch(keymap->mode) {
    case ModeIdle:
        if(action == ActionVoiceStart) {
            keymap_set_mode(keymap, ModeVoice);
        }
        return action;
    case ModeVoice:
        if(!ok_released) {
            return ActionNone;
        }
        keymap_set_mode(keymap, ModeIdle);
        return ActionVoiceStop;
    case ModeVoiceCut:
        if(ok_released) {
            keymap_set_mode(keymap, ModeNoLink);
        }
        break;
    case ModeNoLink:
        if(key == KeymapKeyRight && event == KeymapEventLong) {
            Link link = keymap_other_links[keymap->link];
            keymap_set_screen(keymap, ModeNoLink, link);
            return keymap_use_actions[link];
        }
        break;
    case ModeStopped:
        if(event != KeymapEventPress) {
            break;
        }
        if(key == KeymapKeyBack) {
            return ActionExit;
        }
        keymap_set_mode(keymap, ModeNoLink);
        return ActionStartOver;
    }

    return action == ActionExit ? ActionExit : ActionNone;
}

/* ModeStopped stays: stopping waits for the radio to go idle (furi_hal_bt.c:261-267), and stopping
 * reports a link down itself (bt.c:312), so a report read there is stale. */
Action keymap_set_connected(Keymap* keymap, bool connected) {
    switch(keymap->mode) {
    case ModeIdle:
        if(!connected) {
            keymap_set_mode(keymap, ModeNoLink);
        }
        break;
    case ModeVoice:
        if(!connected) {
            keymap_set_mode(keymap, ModeVoiceCut);
            return ActionVoiceStop;
        }
        break;
    case ModeNoLink:
    case ModeVoiceCut:
        if(connected) {
            keymap_set_mode(keymap, ModeIdle);
        }
        break;
    case ModeStopped:
        break;
    }
    return ActionNone;
}

Action keymap_time_out(Keymap* keymap) {
    if(keymap_wait_ms(keymap) == 0) {
        return ActionNone;
    }
    keymap_set_mode(keymap, ModeStopped);
    return ActionGiveUp;
}

uint32_t keymap_wait_ms(const Keymap* keymap) {
    return keymap->mode == ModeNoLink || keymap->mode == ModeVoiceCut ? keymap_link_wait_ms : 0;
}

Mode keymap_mode(const Keymap* keymap) {
    return keymap->mode;
}

Link keymap_link(const Keymap* keymap) {
    return keymap->link;
}
