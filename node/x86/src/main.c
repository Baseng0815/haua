#include "opcua.h"
#include <led/effects/effects.h>
#include <led/hal.h>
#include <led/mock_strip.h>
#include <led/opcua.h>
#include <node/node.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

	node->led_subsystem.num_strips = NUM_STRIPS;
	node->led_subsystem.strips =
		malloc(sizeof(*node->led_subsystem.strips) * NUM_STRIPS);

	for (i = 0; i < NUM_STRIPS; i++) {
		struct strip *strip = &node->led_subsystem.strips[i];

		led_hal_init(&strip_configs[i], strip);

		strip->info.position.group = i * 3;
	}

	node_run(node);

	for (i = 0; i < NUM_STRIPS; i++) {
		led_hal_deinit(&strips[i]);
	}

	node_destroy(node);

	return 0;
}
