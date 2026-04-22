cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(TARGET logr)
    return()
endif()

project(Logr)

set(BINARY_NAME logr)
set(CMAKE_CXX_STANDARD 23)

set(SOURCE_FILES
    src/Logger.cpp
)

set(HEADER_FILES
    include/logr/defines.h
    include/logr/Logger.h
)

add_library(${BINARY_NAME} OBJECT ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${BINARY_NAME} PUBLIC include)
