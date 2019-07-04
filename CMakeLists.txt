cmake_minimum_required(VERSION 3.10)

# Project name
project(eznot)

# Project files
set(SOURCE_FILES src/main.c)
set(HEADER_FILES)

# Project targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
