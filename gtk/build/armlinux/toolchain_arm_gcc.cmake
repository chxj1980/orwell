# Target system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabihf)

# Cross compiler
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-gnueabihf)

# Search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(THREADS_PTHREAD_ARG "0" CACHE STRING "Result from TRY_RUN" FORCE)