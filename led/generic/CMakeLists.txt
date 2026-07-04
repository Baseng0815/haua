add_library(haua_led_generic STATIC src/led.c src/effects/effects.c src/effects/solid.c)
add_library(haua::led_generic ALIAS haua_led_generic)

target_include_directories(haua_led_generic PUBLIC include)
target_compile_features(haua_led_generic PUBLIC c_std_99)

add_executable(led_example examples/example.c)
target_link_libraries(led_example PRIVATE haua::led_generic)
