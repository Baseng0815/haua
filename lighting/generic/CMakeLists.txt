add_library(haua_lighting_generic STATIC src/lighting.c)
add_library(haua::lighting_generic ALIAS haua_lighting_generic)

target_include_directories(haua_lighting_generic PUBLIC include)
target_compile_features(haua_lighting_generic PUBLIC c_std_99)

add_executable(lighting_example examples/example.c)
target_link_libraries(lighting_example PRIVATE haua::lighting_generic)
