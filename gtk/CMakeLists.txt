cmake_minimum_required(VERSION 3.1.0)

project(orwell)

find_library(AVCODEC_LIBRARY avcodec)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARY swscale)

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

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0) #sudo apt-get install libgtkmm-3.0-dev	

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(include ${GTKMM_INCLUDE_DIRS})


#find_package(Qt5 COMPONENTS Qml Quick Gui Widgets Multimedia MultimediaWidgets) #OpenGLWidgets) 

#set(REACT_NATIVE_REACTQT_DIR "../react-native-desktop/ReactQt/runtime/src/") 
add_subdirectory(../myRtspClient myrtsp)
include_directories(../myRtspClient/myRtspClient/include)
include_directories(../cpp/common/ ../cpp/desktop/)
include_directories(..)
#include_directories(${REACT_NATIVE_REACTQT_DIR})
#include_directories(${REACT_NATIVE_REACTQT_DIR}/layout)

#qt5_add_resources(QRCS resources.qrc) #what this does?
#qt5_add_resources(qml_QRC qml.qrc)    #what this does?

#add_subdirectory(../react-native-desktop RND)
file(GLOB orwell_gtk3_SRCS "*.cpp")

add_executable(orwell ${orwell_gtk3_SRCS})
#add_dependencies(orwell react-native)

#TODO: decide either qtquick or qt widgets libs
target_link_libraries( orwell PRIVATE ${AVCODEC_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY} 
		       Boost::thread myRtspClient pthread ${GTKMM_LIBRARIES} ${GLEW_LIBRARY} ${GLU_LIBRARY} ${GL_LIBRARY} ${X11_LIBRARY})

#RUN WITH:
#/home/lz/cmake-3.14.0-rc2-Linux-x86_64/bin/cmake -DCMAKE_PREFIX_PATH=/home/lz/Qt5.11.2/5.11.2/gcc_64 -DCMAKE_BUILD_TYPE=Debug .
#	g++ -o tutorial2 tutorial2.cpp ../evg/openglarea.cpp ../evg/program.cpp ../evg/shader.cpp -std=c++11 $(shell pkg-config --cflags --libs gtkmm-3.0) -lGLEW -lGLU -lGL -lX11 -I..
