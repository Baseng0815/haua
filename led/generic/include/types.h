#pragma once

#include <stdint.h>

typedef uint32_t led_time_t;
typedef uint32_t led_brightness_t;
typedef uint32_t led_group_t;
typedef uint32_t led_index_t;

enum led_onoff_state {
        LED_ON,
        LED_OFF,
};

typedef enum {
        LED_ERROR_OK,
        LED_ERROR_UNKNOWN,
} led_error_t;
