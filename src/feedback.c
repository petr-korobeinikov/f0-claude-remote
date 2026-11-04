#include "feedback.h"

bool feedback_vibrates(Action action) {
    return action != ActionNone && action != ActionVoiceStop && action != ActionExit;
}

bool feedback_flashes(Action action) {
    return action == ActionEffortHigh || action == ActionEffortMax || action == ActionClear ||
           action == ActionEsc;
}
