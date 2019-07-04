cmake_minimum_required(VERSION 3.10)

# Include log.c
add_definitions(-DLOG_USE_COLOR)
add_library(log.c log.c/src/log.c)
target_include_directories(log.c PUBLIC log.c/src/)

# Include uthash
add_library(uthash INTERFACE)
target_include_directories(uthash INTERFACE uthash/include/)
