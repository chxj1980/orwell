cmake_minimum_required(VERSION 3.4.1)

project(orwell)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files

#sudo apt-get install libglew-dev #will install everything below
#find_library(ZLIB_LIBRARY z)
#find_library(LIBDL_LIBRARY dl)

find_library( log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

add_subdirectory(${PROJECT_ROOT}/myRtspClient myrtsp)
include_directories(${PROJECT_ROOT}/myRtspClient/myRtspClient/include)

file(GLOB LIBORWELL_SRCS "${PROJECT_ROOT}/cpp/common/*" )
include_directories(${PROJECT_ROOT}/cpp/common/ ${PROJECT_ROOT}/cpp/desktop/)


set(FFMPEG_SRC_DIR ${PROJECT_ROOT}/deps/ffmpeg)
set(FFMPEG_LIB_DIR ${PROJECT_ROOT}/deps/ffmpeg/build/android/${ANDROID_ABI}/lib)
include_directories(${FFMPEG_SRC_DIR})

add_library(libavutil STATIC IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.a)
add_library(libavcodec STATIC IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.a)
add_library(libswscale STATIC IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.a)
add_library(libswresample STATIC IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.a)

add_library(liborwell ${LIBORWELL_SRCS})

add_dependencies(liborwell myRtspClient)

target_link_libraries(liborwell PUBLIC myRtspClient)

#We must create a target for Android which links needed libraries in PUBLIC mode. 
#If I understood correctly then we can access all these libs from liborwell
#TODO: separate boost and other libs?
