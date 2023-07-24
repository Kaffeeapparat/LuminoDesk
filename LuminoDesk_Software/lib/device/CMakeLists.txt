add_library(device device.cpp)
target_link_libraries(device PUBLIC 
        ledchannel
        effect)