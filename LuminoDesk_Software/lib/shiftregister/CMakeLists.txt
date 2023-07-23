add_library(shiftregister shiftregister.cpp)
target_link_libraries(shiftregister PUBLIC
        hardwaredef
        )