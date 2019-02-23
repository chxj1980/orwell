project(myRtspClient)

include_directories(third_party/Base64_live555/include)
include_directories(third_party/md5/include)
include_directories(third_party/JRTPLIB/src)
include_directories(include)
include_directories(myRtspClient/include)
include_directories(third_party/JTHREAD/src)


add_subdirectory(third_party/JRTPLIB)
#add_dependencies(myRtspClient jrtplib-shared)


file(GLOB SOURCES "myRtspClient/*.cpp" "third_party/Base64_live555/*.cpp" "third_party/md5/*.cpp" "third_party/JTHREAD/*.cpp"  "third_party/JTHREAD/src*.cpp"  "third_party/JTHREAD/jthread/*.cpp")

add_library(myRtspClient SHARED ${SOURCES})
target_link_libraries(myRtspClient jrtplib-shared)
