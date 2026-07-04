#pragma once

#include <stdint.h>

typedef enum {
	NODE_OK,
	NODE_ERROR,
} node_error_t;

typedef void (*node_tick_fn)(uint32_t now_ms, void *ctx);

struct UA_Server;

struct node {
	const char *name;
	struct UA_Server *server;
};

struct node *node_create(const char *name);
void node_destroy(struct node *node);

node_error_t node_run(struct node *node);
void node_stop(struct node *node);
