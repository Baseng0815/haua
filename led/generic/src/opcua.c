#include <led/opcua.h>

#include <led/hal.h>
#include <led/types.h>
#include <node/node.h>
#include <open62541/server.h>
#include <open62541/types.h>
#include <open62541/util.h>
#include <stdlib.h>

#define RETURN_ON_ERR(x)                        \
	{                                       \
		led_error_t __result = x;       \
		if (__result != LED_ERROR_OK) { \
			return __result;        \
		}                               \
	}

struct show_periodic_callback_data {
	struct strip *strip;
};

static led_error_t
expose_strip(struct node_opcua_data *ua_data, struct strip *strip);
static led_error_t
led_opcua_show_periodic(struct node_opcua_data *ua_data, struct strip *strip);

static void show_periodic_callback(UA_Server *server, void *ctx);

static led_error_t define_strip_object(
	UA_NodeId *node_id, struct node_opcua_data *ua_data, struct strip *strip
);

static led_error_t add_strip_name(
	UA_NodeId node_id, struct node_opcua_data *ua_data, struct strip *strip
);

static led_error_t add_strip_num_leds(
	UA_NodeId node_id, struct node_opcua_data *ua_data, struct strip *strip
);

static led_error_t add_strip_position(
	UA_NodeId node_id, struct node_opcua_data *ua_data, struct strip *strip
);

/**
 * Exposes the subsystem as an OPC UA Object Node and registers a callback
 * to show the strip.
 */
led_error_t led_opcua_expose_subsystem(
	struct node_opcua_data *ua_data, struct led_subsystem *subsystem
)
{
	for (size_t strip_i = 0; strip_i < subsystem->num_strips; strip_i++) {
		RETURN_ON_ERR(
			expose_strip(ua_data, &subsystem->strips[strip_i])
		);
	}

	return LED_ERROR_OK;
}

static led_error_t
expose_strip(struct node_opcua_data *ua_data, struct strip *strip)
{
	UA_NodeId strip_object_node_id;
	RETURN_ON_ERR(
		define_strip_object(&strip_object_node_id, ua_data, strip)
	);
	RETURN_ON_ERR(add_strip_name(strip_object_node_id, ua_data, strip));
	RETURN_ON_ERR(add_strip_num_leds(strip_object_node_id, ua_data, strip));
	RETURN_ON_ERR(add_strip_position(strip_object_node_id, ua_data, strip));

	return LED_ERROR_OK;
}

static led_error_t
led_opcua_show_periodic(struct node_opcua_data *ua_data, struct strip *strip)
{
	struct show_periodic_callback_data *data = malloc(sizeof(*data));
	data->strip = strip;

	if (UA_Server_addRepeatedCallback(
		    ua_data->server, show_periodic_callback, data, 100.0, NULL
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}

static void show_periodic_callback(UA_Server *server, void *ctx)
{
	struct show_periodic_callback_data *data = ctx;
	led_hal_show(data->strip);
}

static led_error_t define_strip_object(
	UA_NodeId *strip_id, struct node_opcua_data *ua_data,
	struct strip *strip
)
{
	UA_ObjectAttributes attr = UA_ObjectAttributes_default;
	UA_String object_name = UA_STRING_NULL;
	UA_String_format(&object_name, "LED strip (%s)", strip->info.name);
	UA_LocalizedText localized_object_name = {
		.text = object_name, .locale = UA_STRING("en-US")
	};
	UA_QualifiedName qualified_object_name = { .name = object_name,
						   .namespaceIndex = 0 };

	attr.displayName = localized_object_name;
	if (UA_Server_addObjectNode(
		    ua_data->server, UA_NODEID_NULL, UA_NS0ID(OBJECTSFOLDER),
		    UA_NS0ID(ORGANIZES), qualified_object_name,
		    UA_NS0ID(BASEOBJECTTYPE), attr, NULL, strip_id
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}

static led_error_t add_strip_name(
	UA_NodeId strip_id, struct node_opcua_data *ua_data, struct strip *strip
)
{
	UA_VariableAttributes attr = UA_VariableAttributes_default;
	UA_String strip_name = UA_STRING_ALLOC(strip->info.name);
	UA_Variant_setScalar(
		&attr.value, &strip_name, &UA_TYPES[UA_TYPES_STRING]
	);
	attr.displayName = UA_LOCALIZEDTEXT("en-US", "StripName");
	if (UA_Server_addVariableNode(
		    ua_data->server, UA_NODEID_NULL, strip_id,
		    UA_NS0ID(HASCOMPONENT), UA_QUALIFIEDNAME(1, "StripName"),
		    UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}

static led_error_t add_strip_num_leds(
	UA_NodeId strip_id, struct node_opcua_data *ua_data, struct strip *strip
)
{
	UA_VariableAttributes attr = UA_VariableAttributes_default;
	UA_UInt32 num_leds = strip->info.num_leds;
	UA_Variant_setScalar(&attr.value, &num_leds, &UA_TYPES[UA_TYPES_UINT32]);
	attr.displayName = UA_LOCALIZEDTEXT("en-US", "NumLeds");
	if (UA_Server_addVariableNode(
		    ua_data->server, UA_NODEID_NULL, strip_id,
		    UA_NS0ID(HASCOMPONENT), UA_QUALIFIEDNAME(1, "NumLeds"),
		    UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}

static led_error_t add_strip_position(
	UA_NodeId strip_id, struct node_opcua_data *ua_data, struct strip *strip
)
{
	UA_VariableAttributes attr = UA_VariableAttributes_default;
	UA_UInt32 position[2] = {
		strip->info.position.group,
		strip->info.position.index,
	};
	UA_Variant_setArray(
		&attr.value, position, 2, &UA_TYPES[UA_TYPES_UINT32]
	);
	UA_UInt32 position_dims[1] = { 2 };
	attr.valueRank = UA_VALUERANK_ONE_DIMENSION;
	attr.arrayDimensions = position_dims;
	attr.arrayDimensionsSize = 1;
	attr.displayName = UA_LOCALIZEDTEXT("en-US", "Position");
	if (UA_Server_addVariableNode(
		    ua_data->server, UA_NODEID_NULL, strip_id,
		    UA_NS0ID(HASCOMPONENT), UA_QUALIFIEDNAME(1, "Position"),
		    UA_NS0ID(BASEDATAVARIABLETYPE), attr, NULL, NULL
	    ) != UA_STATUSCODE_GOOD) {
		return LED_ERROR_UNKNOWN;
	}

	return LED_ERROR_OK;
}
