cmake_minimum_required(VERSION 3.10)

# Project name
project(eznot)

# Include directories
include_directories(include)

# Project files
set(SOURCE_FILES src/main.c src/config.c)
set(HEADER_FILES)

# Project targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
