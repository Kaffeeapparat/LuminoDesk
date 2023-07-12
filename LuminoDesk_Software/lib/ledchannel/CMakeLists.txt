
add_library(ledchannel ledchannel.cpp)
target_link_libraries(ledchannel PUBLIC
        hardwaredef
        )