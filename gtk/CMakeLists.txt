cmake_minimum_required(VERSION 3.1.0)

project(orwell)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_AUTOMOC ON) # Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOUIC ON) # Create code from a list of Qt designer ui files

find_package(OpenGL REQUIRED)
#sudo apt install -y libboost-all-dev 
find_package(Boost COMPONENTS thread system REQUIRED) #todo: erase system, not necessary I guess
#-lGLEW -lGLU -lGL -lX11
#sudo apt-get install libglew-dev #will install everything below
find_library(GLEW_LIBRARY GLEW) 
find_library(GLU_LIBRARY GLU)
find_library(GL_LIBRARY GL)
find_library(X11_LIBRARY X11)
find_library(ZLIB_LIBRARY z)
find_library(LIBDL_LIBRARY dl)
find_library(LIB_VDPAU vdpau) #sudo apt install libvdpau-dev
find_library(LIB_VA va)
find_library(LIB_VA_DRM va-drm)
find_library(LIB_VA_X11 va-x11)




#sudo apt install -y libgtkmm-3.0-dev
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) #sudo apt-get install libgtkmm-3.0-dev	

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})

add_subdirectory(../myRtspClient myrtsp)
include_directories(../myRtspClient/myRtspClient/include)
include_directories(../cpp/common/ ../cpp/desktop/)
include_directories(..)

file(GLOB orwell_gtk3_SRCS "*.cpp" "../cpp/common/*")

set(FFMPEG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg)
set(FFMPEG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps/ffmpeg/build/desktop/x86_64/lib)
include_directories(${FFMPEG_SRC_DIR}/build/desktop/x86_64/include)

if (UNIX)
	add_library(AVUTIL_UPDATED_LIB STATIC IMPORTED)
	set_target_properties(AVUTIL_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavutil.a)
	add_library(AVCODEC_UPDATED_LIB STATIC IMPORTED)
	set_target_properties(AVCODEC_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libavcodec.a)
	add_library(SWS_SCALE_UPDATED_LIB STATIC IMPORTED)
	set_target_properties(SWS_SCALE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswscale.a)
	add_library(SWRESAMPLE_UPDATED_LIB STATIC IMPORTED)
	set_target_properties(SWRESAMPLE_UPDATED_LIB PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_DIR}/libswresample.a)
endif (UNIX)

if (WIN32)
    #to be done, and add macOS too
endif (WIN32)

add_executable(orwell ${orwell_gtk3_SRCS})

target_link_libraries( orwell PRIVATE
	Boost::thread 
	AVCODEC_UPDATED_LIB AVUTIL_UPDATED_LIB SWS_SCALE_UPDATED_LIB SWRESAMPLE_UPDATED_LIB ${ZLIB_LIBRARY} ${LIBDL_LIBRARY} ${LIB_VDPAU} ${LIB_VA} ${LIB_VA_DRM} ${X11_LIBRARY} ${LIB_VA_X11} #zlib MUST come AFTER ffmpeg
	myRtspClient pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY})

