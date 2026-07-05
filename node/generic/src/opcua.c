#include "opcua.h"

#include <open62541/server.h>

#include <node/opcua.h>
#include <node/node.h>
#include <led/opcua.h>

#define RETURN_ON_ERR(x)                   \
	{                                  \
		node_error_t __result = x; \
		if (__result != NODE_OK) { \
			return __result;   \
		}                          \
	}

static node_error_t
define_node_object(UA_NodeId *node_object_id, struct node *node);

static node_error_t
add_node_description(UA_NodeId node_object_id, struct node *node);

static node_error_t
add_node_position(UA_NodeId node_object_id, struct node *node);

node_error_t node_opcua_expose_node(struct node *node)
{
	UA_NodeId node_object_id;
	RETURN_ON_ERR(define_node_object(&node_object_id, node));
	RETURN_ON_ERR(add_node_description(node_object_id, node));
	RETURN_ON_ERR(add_node_position(node_object_id, node));

	RETURN_ON_ERR(
		led_opcua_expose_subsystem(&node->ua_data, &node->led_subsystem)
	)

	return NODE_OK;
}

static node_error_t
define_node_object(UA_NodeId *node_object_id, struct node *node)
{
	(void)node;
	*node_object_id = UA_NODEID_NULL;

	return NODE_OK;
}

static node_error_t
add_node_description(UA_NodeId node_object_id, struct node *node)
{
	(void)node_object_id;
	(void)node;

	return NODE_OK;
}

static node_error_t
add_node_position(UA_NodeId node_object_id, struct node *node)
{
	(void)node_object_id;
	(void)node;

	return NODE_OK;
}
