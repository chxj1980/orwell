set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}
                                                     \"MediaPlayerManager\" PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/mediaplayermanager.cpp
                                              PARENT_SCOPE)

#set(ORWELL_DIR ../../../../../)
#add_subdirectory(${ORWELL_DIR} orwell_subdir)

#set(REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS ${REACT_NATIVE_DESKTOP_EXTERNAL_PROJECT_DEPS}
                                              orwell
                                              PARENT_SCOPE)

#set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS}
                                              orwell 
                                              PARENT_SCOPE)

#set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS}
                                              ${ORWELL_DIR} ${ORWELL_DIR}/myRtspClient/myRtspClient/include
                                              ${ORWELL_DIR}/myRtspClient/
                                              ${ORWELL_DIR}/jrtplib
                                              PARENT_SCOPE)


include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)