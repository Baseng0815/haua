#include <node/node.h>

#include <open62541/server.h>

#include <stdlib.h>
#include <string.h>

struct node *node_create(char *name)
{
	struct node *node = malloc(sizeof(*node));

	node->name = malloc(strlen(name) + 1);
	strcpy(node->name, name);

	return node;
}

void node_destroy(struct node *node)
{
	UA_Server_delete(node->ua_data.server);
	free(node);
}

node_error_t node_run(struct node *node)
{
	node->ua_data.server = UA_Server_new();
	node_opcua_expose_node(node);
	UA_Server_runUntilInterrupt(node->ua_data.server);

	return NODE_ERROR_OK;
}

void node_stop(struct node *node)
{
	UA_Server_delete(node->ua_data.server);
}
