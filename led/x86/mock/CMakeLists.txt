add_library(haua_led_x86_mock STATIC src/hal.c)
add_library(haua::led_x86_mock ALIAS haua_led_x86_mock)

target_include_directories(haua_led_x86_mock PUBLIC include)
target_link_libraries(haua_led_x86_mock PUBLIC haua::led_generic)
target_compile_features(haua_led_x86_mock PUBLIC c_std_99)
