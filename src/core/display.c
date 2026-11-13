#include "display.h"

bool display_lit(Mode mode) {
    switch(mode) {
    case ModeIdle:
    case ModeVoice:
    case ModeNoLink:
    case ModeVoiceCut:
        return true;
    case ModeStopped:
        break;
    }
    return false;
}
