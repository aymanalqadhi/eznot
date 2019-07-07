cmake_minimum_required(VERSION 3.10)

# Request message tests
add_executable(request_message_tests check_request_message.c
									 ../src/request_message.c)
target_link_libraries(request_message_tests log.c check)

# Subscribers hashtable tests
add_executable(subscribers_hashtable_tests check_subscribers_hashtable.c
										   ../src/memory.c ../src/subscribers.c)
target_link_libraries(subscribers_hashtable_tests log.c check uv uthash)

# Publishers hashtable tests
add_executable(publishers_hashtable_tests check_publishers_hashtable.c
										   ../src/memory.c ../src/publisher.c)
target_link_libraries(publishers_hashtable_tests log.c check uthash)
