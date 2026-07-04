add_library(haua_node_generic STATIC src/node.c)
add_library(haua::node_generic ALIAS haua_node_generic)

target_include_directories(haua_node_generic PUBLIC include)
target_link_libraries(haua_node_generic PRIVATE open62541::open62541)
target_compile_features(haua_node_generic PUBLIC c_std_99)
