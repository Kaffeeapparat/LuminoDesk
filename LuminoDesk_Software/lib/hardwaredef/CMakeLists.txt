add_library(hardwaredef hardwaredef.c)
target_link_libraries(hardwaredef PUBLIC
        pico_stdlib
        hardware_spi
        hardware_gpio
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_cyw43_arch_none
        testroutines
        )