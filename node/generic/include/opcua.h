#pragma once

#include "open62541/types.h"
#include <node/types.h>
#include <open62541/server.h>

struct node;

/**
 * OPC UA data the node needs to manage the server and expose itself.
 */
struct node_opcua_data {
        struct UA_Server *server;
        UA_NodeId node_object_type;
        UA_NodeId node_object_instance;
};

/**
 * Exposes the node and its attributes and subsystems as OPC UA nodes.
 */
node_error_t node_opcua_expose_node(struct node *node);
