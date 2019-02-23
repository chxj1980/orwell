set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}
  \"OrwellMediaPlayer\" PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/orwellmediaplayer.cpp
                                              ${CMAKE_CURRENT_SOURCE_DIR}/orwellmediaplayermanager.cpp PARENT_SCOPE)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
