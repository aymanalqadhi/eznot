cmake_minimum_required(VERSION 3.10)

# Project name
project(eznot)

# Include directories
include_directories(include)

# Project files
set(SOURCE_FILES src/main.c src/config.c src/server.c src/memory.c src/address.c
				 src/callbacks.c src/request_message.c src/subscribers.c
				 src/publisher.c src/jobs/send_not.c src/jobs_runner.c)

# Include subprojects
add_subdirectory(lib)
add_subdirectory(tests)

# Add definitions
add_definitions(-DDEBUG)

# Project targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} log.c uthash uv cthreadpool pthread)
