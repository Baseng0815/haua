#include <effects/solid.h>

void effect_solid_apply(
	const struct solid_params *params, const led_time_t time,
	struct strip *strip
)
{
	for (size_t led_i = 0; led_i < strip->info.num_leds; led_i++) {
		strip->state.colors[led_i] = params->color;
	}
}
