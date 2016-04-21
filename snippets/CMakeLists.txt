cmake_minimum_required(VERSION 3.5)
project(mustached_octo_nemesis)

################################################################################
# Compiler Flags
################################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")

################################################################################
# Add subdirectories.
###################################################################k#############
file(GLOB SUBDIRS "." "*")
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${SUBDIR})
        ADD_SUBDIRECTORY(${SUBDIR})
    endif()
endforeach()
