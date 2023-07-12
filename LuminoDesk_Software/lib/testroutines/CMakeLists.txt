
add_library(testroutines testroutines.cpp)
target_link_libraries(testroutines PUBLIC
        pico_stdlib
        hardware_gpio
        hardware_pio
        pico_cyw43_arch_none
        )