cmake_minimum_required(VERSION 3.10.2)

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

add_subdirectory(${PROJECT_ROOT}/myRtspClient _myRtspClient)
include_directories(${PROJECT_ROOT}/myRtspClient/myRtspClient/include)

file(GLOB LIBORWELL_SRCS "${PROJECT_ROOT}/cpp/common/*" )
include_directories(${PROJECT_ROOT}/cpp/common/ ${PROJECT_ROOT}/cpp/desktop/)

#set(RTP_SUPPORT_IFADDRS FALSE)

set(FFMPEG_INCLUDE_DIR ${PROJECT_ROOT}/deps/ffmpeg/build/android/${ANDROID_ABI}/include)
include_directories(${FFMPEG_INCLUDE_DIR})
set(FFMPEG_LIB_DIR ${PROJECT_ROOT}/deps/ffmpeg/build/android/${ANDROID_ABI}/lib)

add_library(libavutil SHARED IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.so)
add_library(libavcodec SHARED IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.so)
add_library(libswscale SHARED IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.so)
add_library(libswresample SHARED IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.so)

add_library(liborwell SHARED ${LIBORWELL_SRCS})

add_dependencies(liborwell myRtspClient)

target_link_libraries(liborwell PUBLIC myRtspClient log-lib)

#We must create a target for Android which links needed libraries in PUBLIC mode. 
#If I understood correctly then we can access all these libs from liborwell
