cmake_minimum_required(VERSION 3.10)

project(libwav VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")

add_library(libwav STATIC 
    src/parse.c
)

target_include_directories(libwav PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(libwav PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)
