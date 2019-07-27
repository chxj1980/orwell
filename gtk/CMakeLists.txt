cmake_minimum_required(VERSION 3.1.0)

project(orwell)

#find_library(AVCODEC_LIBRARY avcodec)
#find_library(AVUTIL_LIBRARY avutil)
#find_library(SWSCALE_LIBRARY swscale)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files

find_package(OpenGL REQUIRED)
find_package(Boost COMPONENTS thread system REQUIRED) #todo: erase system, not necessary I guess
#-lGLEW -lGLU -lGL -lX11
find_library(GLEW_LIBRARY GLEW) #sudo apt-get install libglew-dev
find_library(GLU_LIBRARY GLU)
find_library(GL_LIBRARY GL)
find_library(X11_LIBRARY X11)
find_library(ZLIB_LIBRARY z)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) #sudo apt-get install libgtkmm-3.0-dev	

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

add_subdirectory(../myRtspClient myrtsp)
include_directories(../myRtspClient/myRtspClient/include)
include_directories(../cpp/common/ ../cpp/desktop/)
include_directories(..)

#add_subdirectory(../react-native-desktop RND)
file(GLOB orwell_gtk3_SRCS "*.cpp" "../cpp/common/*")

#Modern FFMPEG
include(ExternalProject)
set(FFMPEG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg)
set(FFMPEG_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg_build)
ExternalProject_Add(ffmpeg PREFIX ${FFMPEG_SRC_DIR}
					#URL ...
    				#URL_HASH SHA2=...
					SOURCE_DIR ${FFMPEG_SRC_DIR}
					CONFIGURE_COMMAND sh ${FFMPEG_SRC_DIR}/configure #sudo apt-get install yasm				
					BINARY_DIR ${FFMPEG_BINARY_DIR}
					BUILD_COMMAND "make"
					INSTALL_COMMAND "")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg)

#Modern zlib (or libz, don't know) is required by ffmpeg, on ubuntu mine didn't work
#set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/zlib-1.2.11)
#add_subdirectory(${ZLIB_DIR} _zlib)
#include_directories(${ZLIB_DIR})

if (UNIX)
	add_library(AVUTIL_UPDATED_LIB SHARED IMPORTED)
	set_target_properties(AVUTIL_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libavutil/libavutil.a)
	add_library(AVCODEC_UPDATED_LIB SHARED IMPORTED)
	set_target_properties(AVCODEC_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libavcodec/libavcodec.a)
	add_library(SWS_SCALE_UPDATED_LIB SHARED IMPORTED)
	set_target_properties(SWS_SCALE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libswscale/libswscale.a)
	add_library(SWRESAMPLE_UPDATED_LIB SHARED IMPORTED)
	set_target_properties(SWRESAMPLE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_BINARY_DIR}/libswresample/libswresample.a)
	endif (UNIX)

if (WIN32)
    #to be done, and add macOS too
endif (WIN32)

add_executable(orwell ${orwell_gtk3_SRCS})
add_dependencies(orwell ffmpeg)# zlib)

#${AVCODEC_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY} #OLD from system libs
target_link_libraries( orwell PRIVATE
			   Boost::thread 
			   AVCODEC_UPDATED_LIB AVUTIL_UPDATED_LIB SWS_SCALE_UPDATED_LIB SWRESAMPLE_UPDATED_LIB ${ZLIB_LIBRARY} #zlib MUST come AFTER ffmpeg
		       myRtspClient pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY} ${X11_LIBRARY})

#TODO: create a target for Android which links needed libraries in PUBLIC mode. Ff I understood correctly that's what needs to be done

#RUN WITH:
#/home/lz/cmake-3.14.0-rc2-Linux-x86_64/bin/cmake -DCMAKE_PREFIX_PATH=/home/lz/Qt5.11.2/5.11.2/gcc_64 -DCMAKE_BUILD_TYPE=Debug .
#	g++ -o tutorial2 tutorial2.cpp ../evg/openglarea.cpp ../evg/program.cpp ../evg/shader.cpp -std=c++11 $(shell pkg-config --cflags --libs gtkmm-3.0) -lGLEW -lGLU -lGL -lX11 -I..
