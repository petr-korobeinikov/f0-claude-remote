#pragma once

#include <stdbool.h>

typedef struct Backlight Backlight;

Backlight* backlight_alloc(bool lit);

void backlight_free(Backlight* backlight);

void backlight_set(Backlight* backlight, bool lit);
