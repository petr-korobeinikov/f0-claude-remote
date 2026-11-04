#pragma once

#include <input/input.h>

#include "keymap.h"

Action keymap_input_handle(Keymap* keymap, const InputEvent* event);
