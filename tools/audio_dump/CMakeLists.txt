cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/../../release" CACHE PATH "Installation Directory")
endif()
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

project(pre_aqt_test)

set (FACE_REC_RT_INFO_LIB_PATH
    ../release/x86
    )

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}              
                )

#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/${FACE_REC_RT_INFO_LIB_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

set(TLHMI_AUDIO_DUMP_PARSE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/parse_audio_runtime_info.cpp
)

add_executable(parse_audio_runtime_info ${TLHMI_AUDIO_DUMP_PARSE_SRC})