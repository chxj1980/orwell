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
find_library(MLIB_LIBRARY m)
find_library(LIBDL_LIBRARY dl)
find_library(LIBDRM_LIBRARY drm)

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

message(STATUS "------------ COMPILE_TARGET: ${COMPILE_TARGET}")

if (COMPILE_TARGET STREQUAL "JETSON_NANO")
	message(STATUS "Compiling for JETSON_NANO")
	include_directories(../cpp/l4tmultimedia)
	#include_directories(/usr/include/aarch64-linux-gnu)
	#TODO: change to system's folder, but where?
	include_directories(../deps/tegra_multimedia_api/include)
	include_directories(../deps/tegra_multimedia_api/include/libjpeg-8b)
	include_directories(/usr/include/libdrm)
	find_library(LIB_NVBUF nvbuf_utils PATHS /usr/lib/aarch64-linux-gnu/tegra)
	find_library(LIB_V4L2 v4l2)
	find_library(LIB_NVJPEG nvjpeg PATHS /usr/lib/aarch64-linux-gnu/tegra)
	find_library(LIB_DRM drm PATHS /usr/lib/aarch64-linux-gnu/tegra)
	find_library(LIB_EGL EGL PATHS /usr/lib/aarch64-linux-gnu/tegra)
	#find_library(LIB_V4L2_VIDEO_CODEC v4l2_nvvideocodec PATHS /usr/lib/aarch64-linux-gnu/tegra)
	file(GLOB ORWELL_SOURCES orwellNvidia.cpp OpenGLArea2.cpp OpenglSmartRenderer3.cpp NvidiaRendererEGL.cpp Program.cpp 
	Shader.cpp NVidiaRenderer.cpp
	"../cpp/common/*.cpp" "../cpp/l4tmultimedia/*.cpp" 
	"../deps/tegra_multimedia_api/samples/common/classes/*.cpp")
else (COMPILE_TARGET STREQUAL "DESKTOP_x86_64")
	message(STATUS "Compiling for DESKTOP_x86_64")
	file(GLOB ORWELL_SOURCES orwellFfmpeg.cpp OpenGLArea2.cpp OpenglSmartRenderer3.cpp Program.cpp Shader.cpp "../cpp/common/*")
endif()

set(FFMPEG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg/build/desktop/${CMAKE_SYSTEM_PROCESSOR}/lib)
set(FFMPEG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg/build/desktop/${CMAKE_SYSTEM_PROCESSOR}/include)
include_directories(${FFMPEG_INCLUDE_DIR})

message(STATUS "FFMPEG_LIB_DIR: ${FFMPEG_LIB_DIR}")

add_library(AVDEVICE_LIB STATIC IMPORTED)
set_target_properties(AVDEVICE_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavdevice.a)
add_library(AVFORMAT_LIB STATIC IMPORTED)
set_target_properties(AVFORMAT_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavformat.a)
add_library(AVUTIL_LIB STATIC IMPORTED)
set_target_properties(AVUTIL_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.a)
add_library(AVCODEC_LIB STATIC IMPORTED)
set_target_properties(AVCODEC_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.a)
add_library(SWS_SCALE_LIB STATIC IMPORTED)
set_target_properties(SWS_SCALE_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.a)
add_library(SWRESAMPLE_LIB STATIC IMPORTED)
set_target_properties(SWRESAMPLE_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.a)

add_executable(orwell ${ORWELL_SOURCES})

if (COMPILE_TARGET STREQUAL "JETSON_NANO")
	target_link_libraries(orwell PRIVATE 
	AVCODEC_LIB AVUTIL_LIB SWS_SCALE_LIB SWRESAMPLE_LIB 
	${ZLIB_LIBRARY} ${LIBDL_LIBRARY} ${X11_LIBRARY} ${LIB_VDPAU} ${LIB_VA} ${LIB_VA_DRM} ${LIB_VA_X11} ${LIB_V4L2} ${LIB_NVJPEG} ${LIB_DRM} ${LIB_EGL} ${LIB_NVBUF}
	myRtspClient-static pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY} epoxy)
else (COMPILE_TARGET STREQUAL "GENERIC_x86_64")
	target_link_libraries(orwell PRIVATE 
	AVCODEC_LIB AVUTIL_LIB SWS_SCALE_LIB SWRESAMPLE_LIB 
	${ZLIB_LIBRARY} ${LIBDL_LIBRARY} ${X11_LIBRARY} ${LIB_VDPAU} ${LIB_VA} ${LIB_VA_DRM} ${LIB_VA_X11}
	myRtspClient-static pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY} epoxy)
endif()
	#${ZLIB_LIBRARY} ${LIBDL_LIBRARY} ${X11_LIBRARY} ${LIB_VDPAU} ${LIB_VA} ${LIB_VA_DRM} ${LIB_VA_X11} ${LIBDRM_LIBRARY} 

#LAB is for testing things
file(GLOB LAB_SOURCES main3.cpp ../cpp/common/SLog.cpp)
add_executable(lab ${LAB_SOURCES})
target_link_libraries(lab pthread)
