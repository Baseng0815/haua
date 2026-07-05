#pragma once

#include <node/opcua.h>
#include <led/subsystem.h>

/**
 * A wrapper struct that bundles node and subsystem data together.
 */
struct node {
        char *name;

        /** Data the generic node needs to keep about the OPC UA server it
         * is hosting.
         */
        struct node_opcua_data ua_data;

        struct led_subsystem led_subsystem;
};

struct node *node_create(char *name);
void node_destroy(struct node *node);

/** 
 * Run the node. This sets up the OPC UA information model based on the populated
 * subsystems and exposes the node.
 */
node_error_t node_run(struct node *node);
void node_stop(struct node *node);
