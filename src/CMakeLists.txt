add_executable(queueproc main.cpp message.cpp controller.cpp table.cpp)
target_include_directories(queueproc PUBLIC ${CMAKE_SOURCE_DIR}/inc)
