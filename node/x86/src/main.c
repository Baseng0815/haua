#include "effects/effects.h"
#include <led_opcua.h>
#include <mock_strip.h>
#include <node.h>

#include <hal.h>
#include <stddef.h>
#include <stdint.h>

static const struct mock_strip_config strip_configs[] = {
	{ .name = "living-room", .num_leds = 30 },
	{ .name = "kitchen", .num_leds = 60 },
};

#define NUM_STRIPS (sizeof(strip_configs) / sizeof(strip_configs[0]))

static struct strip strips[NUM_STRIPS];

static void tick(uint32_t now_ms, void *ctx)
{
	size_t i;

	(void)now_ms;
	(void)ctx;

	for (i = 0; i < NUM_STRIPS; i++) {
		led_hal_show(&strips[i]);
	}
}

int main(void)
{
	struct node *node;
	size_t i;

	node = node_create("haua-x86-mock");
	if (!node) {
		return 1;
	}

	for (i = 0; i < NUM_STRIPS; i++) {
		struct strip *strip = &strips[i];

		led_hal_init(&strip_configs[i], strip);
		led_opcua_expose_strip(node, strip);
		led_opcua_show_periodic(node, strip);

                strip->info.position.group = i * 3;

		struct solid_params solid_params = {
			.color = { .r = 100, .b = 125, .g = 30 }
		};

		struct effect_params effect_params = { .kind = EFFECT_SOLID,
						       .params.solid_params =
							       solid_params };

		struct effect effect = { .params = effect_params, .time = 0 };

		effect_apply(&effect, strip);
	}

        node_run(node);

	for (i = 0; i < NUM_STRIPS; i++) {
		led_hal_deinit(&strips[i]);
	}

	node_destroy(node);

	return 0;
}
