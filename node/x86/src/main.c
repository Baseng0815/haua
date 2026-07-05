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

int main(void)
{
        struct node *node = node_create("haua-x86-mock");
        if (!node) {
                return 1;
        }

        node->led_subsystem.num_strips = NUM_STRIPS;
        node->led_subsystem.strips =
                malloc(sizeof(*node->led_subsystem.strips) * NUM_STRIPS);

        for (size_t strip_i = 0; strip_i < NUM_STRIPS; strip_i++) {
                struct strip *strip = &node->led_subsystem.strips[strip_i];

                led_hal_init(&strip_configs[strip_i], strip);

                strip->info.position.group = strip_i * 3;
        }

        node_run(node);

        for (size_t strip_i = 0; strip_i < node->led_subsystem.num_strips;
             strip_i++) {
                led_hal_deinit(&node->led_subsystem.strips[strip_i]);
        }

        node_destroy(node);

        return 0;
}
