
add_library(ledchannel ledchannel.c)
target_link_libraries(ledchannel PUBLIC
        pico_stdlib
        hardwaredef
        )