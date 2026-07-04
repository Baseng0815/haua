add_library(haua_led_opcua STATIC src/led_opcua.c)
add_library(haua::led_opcua ALIAS haua_led_opcua)

target_include_directories(haua_led_opcua PUBLIC include)
target_link_libraries(haua_led_opcua PUBLIC haua::led_generic)
target_link_libraries(haua_led_opcua PRIVATE open62541::open62541 haua::node_generic)
target_compile_features(haua_led_opcua PUBLIC c_std_99)
