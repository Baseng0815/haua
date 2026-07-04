#pragma once

#include <hal.h>

struct node;

/**
 * Exposes the strip as an OPC UA Object Node.
 */
led_error_t
led_opcua_expose_strip(struct node *node, struct strip *strip);

/**
 * Registers a periodic callback with node that shows the strip.
 */
led_error_t led_opcua_show_periodic(struct node *node, struct strip *strip);
