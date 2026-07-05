#pragma once

#include "effects/effects.h"
#include <stdbool.h>
#include <stddef.h>

struct strip;

enum effect_onoff_state { EFFECT_ON, EFFECT_OF };

struct led_subsystem {
        struct strip *strips;
        size_t num_strips;

        struct effect current_effect;
        enum effect_onoff_state effect_onoff_state;
};
