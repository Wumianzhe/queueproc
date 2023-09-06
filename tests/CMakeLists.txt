# feels like a bad way to do tests, but I don't know a good way
add_executable(msg_parse testmain.cpp parsetest.cpp ../src/message.cpp)
target_include_directories(msg_parse PUBLIC ${CMAKE_SOURCE_DIR}/doctest/doctest ${CMAKE_SOURCE_DIR}/inc)
