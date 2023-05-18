# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(LuminoDesk_Software C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(LuminoDesk_Software main.c )
add_subdirectory(lib/hardwaredef)
include_directories(/lib/hardwaredef)
add_subdirectory(lib/testroutines)
include_directories(/lib/testroutines)
add_subdirectory(lib/ledchannel)
include_directories(/lib/ledchannel)



pico_set_program_name(LuminoDesk_Software "LuminoDesk_Software")
pico_set_program_version(LuminoDesk_Software "0.1")

pico_enable_stdio_uart(LuminoDesk_Software 1)
pico_enable_stdio_usb(LuminoDesk_Software 0)

# Add the standard library to the build
target_link_libraries(LuminoDesk_Software
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(LuminoDesk_Software PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(LuminoDesk_Software
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_cyw43_arch_none
        hardwaredef
        testroutines
        ledchannel
        )

pico_add_extra_outputs(LuminoDesk_Software)

