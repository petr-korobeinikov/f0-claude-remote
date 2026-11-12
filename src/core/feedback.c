#include "feedback.h"

bool feedback_vibrates(Action action) {
    switch(action) {
    case ActionEnter:
    case ActionArrowUp:
    case ActionArrowDown:
    case ActionEsc:
    case ActionEffortHigh:
    case ActionEffortMax:
    case ActionClear:
    case ActionVoiceStart:
        return true;
    case ActionNone:
    case ActionVoiceStop:
    case ActionExit:
    case ActionGiveUp:
    case ActionStartOver:
    case ActionUseUsb:
    case ActionUseBluetooth:
        break;
    }
    return false;
}

/* Arrows come in series, and a flash longer than the gap between taps would merge them. */
uint32_t feedback_flash_ms(Action action) {
    switch(action) {
    case ActionArrowUp:
    case ActionArrowDown:
        return 100;
    case ActionEnter:
    case ActionEsc:
    case ActionEffortHigh:
    case ActionEffortMax:
    case ActionClear:
        return 400;
    case ActionNone:
    case ActionVoiceStart:
    case ActionVoiceStop:
    case ActionExit:
    case ActionGiveUp:
    case ActionStartOver:
    case ActionUseUsb:
    case ActionUseBluetooth:
        break;
    }
    return 0;
}
