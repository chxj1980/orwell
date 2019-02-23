# Install script for directory: /home/lz/orwell/myRtspClient/third_party/JRTPLIB/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/jrtplib3" TYPE FILE FILES
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpapppacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpbyepacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpcompoundpacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpcompoundpacketbuilder.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcppacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcppacketbuilder.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcprrpacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpscheduler.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpsdesinfo.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpsdespacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpsrpacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtcpunknownpacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpaddress.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpcollisionlist.h"
    "/home/lz/orwell/orwellJS/desktop/lib/_orwell/myRtspClient/third_party/JRTPLIB/src/rtpconfig.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpdebug.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpdefines.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtperrors.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtphashtable.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpinternalsourcedata.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpipv4address.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpipv4destination.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpipv6address.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpipv6destination.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpkeyhashtable.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtplibraryversion.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpmemorymanager.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpmemoryobject.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtppacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtppacketbuilder.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtppollthread.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtprandom.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtprandomrand48.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtprandomrands.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtprandomurandom.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtprawpacket.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsession.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsessionparams.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsessionsources.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsourcedata.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsources.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpstructs.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtptimeutilities.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtptransmitter.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtptypes_win.h"
    "/home/lz/orwell/orwellJS/desktop/lib/_orwell/myRtspClient/third_party/JRTPLIB/src/rtptypes.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpudpv4transmitter.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpudpv6transmitter.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpbyteaddress.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpexternaltransmitter.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsecuresession.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpsocketutil.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpabortdescriptors.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtpselect.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtptcpaddress.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/rtptcptransmitter.h"
    "/home/lz/orwell/myRtspClient/third_party/JRTPLIB/src/extratransmitters/rtpfaketransmitter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libjrtp.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/home/lz/orwell/orwellJS/desktop/lib/_orwell/myRtspClient/third_party/JRTPLIB/src/libjrtp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libjrtp.so.3.11.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES "/home/lz/orwell/orwellJS/desktop/lib/_orwell/myRtspClient/third_party/JRTPLIB/src/libjrtp.so.3.11.1")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/libjrtp.so.3.11.1")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libjrtp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libjrtp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/libjrtp.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libjrtp.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES "/home/lz/orwell/orwellJS/desktop/lib/_orwell/myRtspClient/third_party/JRTPLIB/src/libjrtp.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/libjrtp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/libjrtp.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/libjrtp.so")
    endif()
  endif()
endif()

