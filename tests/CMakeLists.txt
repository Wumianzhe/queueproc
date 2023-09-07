# feels like a bad way to pick files for test compilation, but i don't know a good one
# better one would be creating a library target and linking with it
add_executable(msg_parse testmain.cpp parsetest.cpp ../src/message.cpp)
add_executable(table_time testmain.cpp tabletest.cpp ../src/table.cpp)
add_executable(controller testmain.cpp controllertest.cpp ../src/controller.cpp ../src/message.cpp ../src/table.cpp)
target_include_directories(msg_parse PUBLIC ${CMAKE_SOURCE_DIR}/doctest/doctest ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(table_time PUBLIC ${CMAKE_SOURCE_DIR}/doctest/doctest ${CMAKE_SOURCE_DIR}/inc)
target_include_directories(controller PUBLIC ${CMAKE_SOURCE_DIR}/doctest/doctest ${CMAKE_SOURCE_DIR}/inc)
