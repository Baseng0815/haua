#pragma once

#include <led/types.h>

struct node_opcua_data;
struct led_subsystem;

/**
 * Exposes the subsystem as an OPC UA Object Node and registers a callback
 * to show the strip.
 */
led_error_t led_opcua_expose_subsystem(
        struct node_opcua_data *ua_data, struct led_subsystem *subsystem
);
