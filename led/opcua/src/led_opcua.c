#include "hal.h"
#include "open62541/util.h"
#include "types.h"
#include <led_opcua.h>

#include <node.h>
#include <open62541/server.h>
#include <stdlib.h>

struct show_periodic_callback_data {
	struct strip *strip;
};

static void show_periodic_callback(UA_Server *server, void *ctx);

led_error_t
led_opcua_expose_strip(struct node *node, struct strip *strip, const char *name)
{

	return LED_ERROR_OK;
}

static void show_periodic_callback(UA_Server *server, void *ctx)
{
	struct show_periodic_callback_data *data = ctx;
	led_hal_show(data->strip);
}

led_error_t led_opcua_show_periodic(struct node *node, struct strip *strip)
{
	struct show_periodic_callback_data *data = malloc(sizeof(*data));
	data->strip = strip;

	if (UA_Server_addRepeatedCallback(
		    node->server, show_periodic_callback, data, 100.0, NULL
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}
