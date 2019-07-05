cmake_minimum_required(VERSION 3.10)

#Request message tests
add_executable(request_message_tests check_request_message.c
									 ../src/request_message.c)
target_link_libraries(request_message_tests log.c check)

