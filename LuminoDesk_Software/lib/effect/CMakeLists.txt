add_library(effect effect.cpp)
target_link_libraries(effect PUBLIC 
    ledchannel
    hardwaredef)