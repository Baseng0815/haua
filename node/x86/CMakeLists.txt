add_executable(haua_node_x86 src/main.c)
target_link_libraries(haua_node_x86 PRIVATE
	haua::node_generic
	haua::led_opcua
	haua::led_x86_mock)
