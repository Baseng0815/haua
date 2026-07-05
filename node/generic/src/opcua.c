#include "opcua.h"

#include "open62541/types.h"
#include "open62541/util.h"
#include "types.h"
#include <open62541/server.h>

#include <node/opcua.h>
#include <node/node.h>
#include <led/opcua.h>

#define RETURN_ON_NODE_ERR(x)                    \
        {                                        \
                node_error_t __result = x;       \
                if (__result != NODE_ERROR_OK) { \
                        return __result;         \
                }                                \
        }

#define RETURN_ON_UA_ERR(x)                           \
        {                                             \
                UA_StatusCode __result = x;           \
                if (__result != UA_STATUSCODE_GOOD) { \
                        return NODE_ERROR_UNKNOWN;    \
                }                                     \
        }

static node_error_t define_node_object(struct node_opcua_data *node);

static node_error_t create_node_object_instance(struct node *node);

static node_error_t add_node_description(struct node *node);

node_error_t node_opcua_expose_node(struct node *node)
{
        UA_NodeId node_object_id;
        RETURN_ON_NODE_ERR(define_node_object(&node->ua_data));
        RETURN_ON_NODE_ERR(create_node_object_instance(node));
        RETURN_ON_NODE_ERR(add_node_description(node));

        RETURN_ON_NODE_ERR(
                led_opcua_expose_subsystem(&node->ua_data, &node->led_subsystem)
        )

        return NODE_ERROR_OK;
}

static node_error_t define_node_object(struct node_opcua_data *ua_data)
{
        UA_ObjectTypeAttributes object_attr = UA_ObjectTypeAttributes_default;
        object_attr.displayName = UA_LOCALIZEDTEXT("en-US", "NodeType");
        RETURN_ON_UA_ERR(UA_Server_addObjectTypeNode(
                ua_data->server, UA_NODEID_NULL, UA_NS0ID(BASEOBJECTTYPE),
                UA_NS0ID(HASSUBTYPE), UA_QUALIFIEDNAME(1, "NodeType"),
                object_attr, NULL, &ua_data->node_object_type
        ));

        UA_VariableAttributes desc_attr = UA_VariableAttributes_default;
        desc_attr.displayName = UA_LOCALIZEDTEXT("en-US", "Description");
        UA_NodeId desc_id;
        RETURN_ON_UA_ERR(UA_Server_addVariableNode(
                ua_data->server, UA_NODEID_NULL, ua_data->node_object_type,
                UA_NS0ID(HASCOMPONENT), UA_QUALIFIEDNAME(1, "Description"),
                UA_NS0ID(BASEDATAVARIABLETYPE), desc_attr, NULL, &desc_id
        ));
        RETURN_ON_UA_ERR(UA_Server_addReference(
                ua_data->server, desc_id, UA_NS0ID(HASMODELLINGRULE),
                UA_NS0EXID(MODELLINGRULE_MANDATORY), true
        ));

        return NODE_ERROR_OK;
}

static node_error_t create_node_object_instance(struct node *node)
{
        UA_ObjectAttributes attr = UA_ObjectAttributes_default;
        attr.displayName = (UA_LocalizedText){
                .text = UA_STRING_ALLOC(node->name),
                .locale = UA_STRING("en-US"),
        };
        RETURN_ON_UA_ERR(UA_Server_addObjectNode(
                node->ua_data.server, UA_NODEID_NULL, UA_NS0ID(OBJECTSFOLDER),
                UA_NS0ID(ORGANIZES), UA_QUALIFIEDNAME_ALLOC(1, node->name),
                node->ua_data.node_object_type, attr, NULL, NULL
        ));

        return NODE_ERROR_OK;
}

static node_error_t add_node_description(struct node *node)
{
        return NODE_ERROR_OK;
}
