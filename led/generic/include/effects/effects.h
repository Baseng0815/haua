#pragma once

#include <led/effects/solid.h>
#include <led/types.h>

enum effect_kind { EFFECT_SOLID };

struct effect_params {
	union {
		struct solid_params solid_params;
	} params;

	enum effect_kind kind;
};

struct effect {
	struct effect_params params;
	led_time_t time;
};

void effect_apply(const struct effect *effect, struct strip *strip);
