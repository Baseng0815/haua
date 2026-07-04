#include <node.h>

#include <open62541/server.h>

#include <stdlib.h>

struct node *node_create(const char *name)
{
	struct node *node = malloc(sizeof(struct node));

	node->name = name;
	node->server = UA_Server_new();

	return node;
}

void node_destroy(struct node *node)
{
	UA_Server_delete(node->server);
	free(node);
}

node_error_t node_run(struct node *node)
{
	UA_Server_runUntilInterrupt(node->server);

	return NODE_OK;
}

void node_stop(struct node *node)
{
	(void)node;
}
