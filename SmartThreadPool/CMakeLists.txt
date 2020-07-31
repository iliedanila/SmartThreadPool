project(ThreadPoolLib)

include_directories(inc)

file(GLOB SOURCES "src/*.cpp")

add_library(smartThreadPool STATIC ${SOURCES})