#pragma once

#include <led/hal.h>
#include <led/types.h>

struct solid_params {
	struct color_rgb color;
};

void effect_solid_apply(const struct solid_params *params,
			const led_time_t time, struct strip *strip);
