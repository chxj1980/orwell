set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}
                                                     \"MediaPlayerManager\" PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/mediaplayermanager.cpp
                                              PARENT_SCOPE)

#Root of the project
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
#Place where lies QT cpp files (are used in react native desktop as modules)
set(QT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../cpp/desktop)
#React QT directory
set(REACT_QT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../node_modules/react-native/ReactQt/runtime/src)

#TODO: use find_package instead of passing the l name

set(REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS ${REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS}
                                              orwell
                                              PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS}
                                              orwell myRtspClient boost_thread boost_system
                                              PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS}
                                              ${QT_DIR}
                                              ${ROOT}/myRtspClient/myRtspClient/include
                                              ${ROOT}/myRtspClient
                                              ${ROOT}/jrtplib
					      ${QT_DIR}/../common
                                              ${REACT_QT_DIR}
                                              ${REACT_QT_DIR}/layout
                                              PARENT_SCOPE)


include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
