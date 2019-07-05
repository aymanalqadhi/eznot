cmake_minimum_required(VERSION 3.10)

# Project name
project(eznot)

# Include directories
include_directories(include)

# Project files
set(SOURCE_FILES src/main.c src/config.c src/server.c src/memory.c src/address.c
				 src/callbacks.c)
set(HEADER_FILES)

# Include subprojects
add_subdirectory(lib)

# Add definitions
add_definitions(-DDEBUG)

# Project targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} log.c uthash uv)
