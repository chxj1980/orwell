cmake_minimum_required(VERSION 3.1.0)

project(orwell)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files

find_package(OpenGL REQUIRED)
#-lGLEW -lGLU -lGL -lX11
#sudo apt-get install libglew-dev #will install everything below
find_library(GLEW_LIBRARY GLEW) 
find_library(GLU_LIBRARY GLU)
find_library(GL_LIBRARY GL)
find_library(X11_LIBRARY X11)
find_library(ZLIB_LIBRARY z)
find_library(LIBDL_LIBRARY dl)
#These driver libraries are only available for x86 or x86_64, arm libs are not compiled with them
if (CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64 OR CMAKE_SYSTEM_PROCESSOR STREQUAL x86)
find_library(LIB_VDPAU vdpau) #sudo apt install libvdpau-dev
find_library(LIB_VA va)
find_library(LIB_VA_DRM va-drm)
find_library(LIB_VA_X11 va-x11)
endif()

#sudo apt install -y libgtkmm-3.0-dev
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) #sudo apt-get install libgtkmm-3.0-dev	

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

add_subdirectory(../myRtspClient _myRtspClient)
include_directories(../myRtspClient/myRtspClient/include)
include_directories(../cpp/common/ ../cpp/desktop/)
include_directories(..)
include_directories(../deps/cxxopts/include)

file(GLOB ORWELL_SOURCES main.cpp OpenGLArea2.cpp OpenglSmartRenderer3.cpp Program.cpp Shader.cpp "../cpp/common/*")

set(FFMPEG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg/build/desktop/${CMAKE_SYSTEM_PROCESSOR}/lib)
set(FFMPEG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg/build/desktop/${CMAKE_SYSTEM_PROCESSOR}/include)
include_directories(${FFMPEG_INCLUDE_DIR})

message(STATUS "FFMPEG_LIB_DIR: ${FFMPEG_LIB_DIR}")

add_library(AVUTIL_LIB STATIC IMPORTED)
set_target_properties(AVUTIL_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.a)
add_library(AVCODEC_LIB STATIC IMPORTED)
set_target_properties(AVCODEC_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.a)
add_library(SWS_SCALE_LIB STATIC IMPORTED)
set_target_properties(SWS_SCALE_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.a)
add_library(SWRESAMPLE_LIB STATIC IMPORTED)
set_target_properties(SWRESAMPLE_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.a)

add_executable(orwell ${ORWELL_SOURCES})

target_link_libraries(orwell PRIVATE 
	AVCODEC_LIB AVUTIL_LIB SWS_SCALE_LIB SWRESAMPLE_LIB 
	${ZLIB_LIBRARY} ${LIBDL_LIBRARY} ${X11_LIBRARY} ${LIB_VDPAU} ${LIB_VA} ${LIB_VA_DRM} ${LIB_VA_X11}
	myRtspClient pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY} epoxy)

#LAB is for testing things
file(GLOB LAB_SOURCES main3.cpp ../cpp/common/SLog.cpp)
add_executable(lab ${LAB_SOURCES})
target_link_libraries(lab pthread)
