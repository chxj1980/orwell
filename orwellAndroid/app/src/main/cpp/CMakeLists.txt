cmake_minimum_required(VERSION 3.4.1)

project(orwell)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files

#sudo apt install -y libboost-all-dev
find_package(Boost COMPONENTS thread system REQUIRED) #todo: erase system, not necessary I guess

#sudo apt-get install libglew-dev #will install everything below
find_library(ZLIB_LIBRARY z)
find_library(LIBDL_LIBRARY dl)


find_library( log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

add_subdirectory(${PROJECT_ROOT}/myRtspClient myrtsp)
include_directories(${PROJECT_ROOT}/myRtspClient/myRtspClient/include)

file(GLOB LIBORWELL_SRCS "${PROJECT_ROOT}/cpp/common/*" )
include_directories(${PROJECT_ROOT}/cpp/common/ ${PROJECT_ROOT}/cpp/desktop/)


set(FFMPEG_SRC_DIR ${PROJECT_ROOT}/deps/ffmpeg)
set(FFMPEG_BINARY_DIR ${PROJECT_ROOT}/deps/ffmpeg)
include_directories(${FFMPEG_SRC_DIR})

add_library(AVUTIL_UPDATED_LIB SHARED IMPORTED)
set_target_properties(AVUTIL_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libavutil/libavutil.a)
add_library(AVCODEC_UPDATED_LIB SHARED IMPORTED)
set_target_properties(AVCODEC_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libavcodec/libavcodec.a)
add_library(SWS_SCALE_UPDATED_LIB SHARED IMPORTED)
set_target_properties(SWS_SCALE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libswscale/libswscale.a)
add_library(SWRESAMPLE_UPDATED_LIB SHARED IMPORTED)
set_target_properties(SWRESAMPLE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libswresample/libswresample.a)

add_library(liborwell ${LIBORWELL_SRCS})

add_dependencies(liborwell myRtspClient)

target_link_libraries(liborwell PUBLIC
        Boost::thread
        AVCODEC_UPDATED_LIB AVUTIL_UPDATED_LIB SWS_SCALE_UPDATED_LIB SWRESAMPLE_UPDATED_LIB ${ZLIB_LIBRARY} ${LIBDL_LIBRARY} #zlib MUST come AFTER ffmpeg
        myRtspClient)

#We must create a target for Android which links needed libraries in PUBLIC mode. 
#If I understood correctly then we can access all these libs from liborwell
#TODO: separate boost and other libs?
