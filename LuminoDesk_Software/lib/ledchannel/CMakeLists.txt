
add_library(ledchannel ledchannel.c)
target_link_libraries(ledchannel PUBLIC
        hardwaredef
        )