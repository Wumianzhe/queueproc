add_library(queueproc message.cpp controller.cpp table.cpp)
target_include_directories(queueproc PUBLIC ${CMAKE_SOURCE_DIR}/inc)
