#pragma once

#include <types.h>
#include <stddef.h>
#include <stdint.h>

struct color_rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

/**
 * The strip position determines the position of the logical strip. It is made up
 * of a group index as well as of an index inside of that group.
 */
struct strip_position {
  led_group_t group;
  led_index_t index;
};

/**
 * The strip state describes a full state of the logical strip. It can be changed
 * directly or through various effects and is flushed to the physical strip via the
 * HAL.
 */
struct strip_state {
	struct color_rgb *colors;
	enum led_onoff_state on_off_state;
	led_brightness_t brightness;
};

/**
 * Strip info describes a logical strip in a device-agnostic manner.
 */
struct strip_info {
	size_t num_leds;
        struct strip_position position;
};

/**
 * The logical strip abstraction contains its current state as well as its
 * immutable properties and driver-specific data.
 */
struct strip {
	struct strip_state state;
	struct strip_info info;
	void *drv;
};

/**
 * Initialize a logical representation of the physical strip given by the configuration.
 * After this call, device-agnostic functions can be called on the configured strip.
 */
led_error_t led_hal_init(const void *config, struct strip *strip);

/**
 * Flush the state of the given logical strip to the physical strip.
 */
led_error_t led_hal_show(struct strip *strip);

/**
 * Deinitialize the logical strip.
 */
led_error_t led_hal_deinit(struct strip *strip);
