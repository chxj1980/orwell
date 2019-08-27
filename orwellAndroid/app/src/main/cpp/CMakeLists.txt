cmake_minimum_required(VERSION 3.10.2)
add_compile_definitions(ANDROID)
project(orwell)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files


find_library(log-lib log)

add_subdirectory(${PROJECT_ROOT}/myRtspClient _myRtspClient)
include_directories(${PROJECT_ROOT}/myRtspClient/myRtspClient/include)

file(GLOB LIBORWELL_SRCS "${PROJECT_ROOT}/cpp/common/*" )
include_directories(${PROJECT_ROOT}/cpp/common/ ${PROJECT_ROOT}/cpp/desktop/)

set(FFMPEG_INCLUDE_DIR ${PROJECT_ROOT}/deps/ffmpeg/build/android/${ANDROID_ABI}/include)
include_directories(${FFMPEG_INCLUDE_DIR})
set(FFMPEG_LIB_DIR ${PROJECT_ROOT}/deps/ffmpeg/build/android/${ANDROID_ABI}/lib)

add_library(libavcodec SHARED IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.so)
add_library(libavutil SHARED IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.so)
add_library(libswscale SHARED IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.so)
add_library(libswresample SHARED IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.so)
#TODO: add libavfilter

add_library(orwell SHARED ${LIBORWELL_SRCS})

add_dependencies(orwell myRtspClient libavcodec libavutil libswscale libswresample)

target_link_libraries(orwell android log myRtspClient libavcodec libavutil libswscale libswresample)
