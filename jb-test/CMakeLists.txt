set(prj_name jb-test)
project(${prj_name})
cmake_minimum_required(VERSION 2.8)

include ("/usr/local/include/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")

set(file_srcs main.cpp jbdungeon.cpp jbmaze.cpp jbmazemask.cpp gameutil.cpp)
set(file_hdrs jbdungeon.h jbmaze.h jbmazemask.h gameutil.h)

add_definitions(-DLINUX)

ADD_EXECUTABLE(${prj_name} ${file_srcs} ${file_hdrs})
TARGET_LINK_LIBRARIES(${prj_name} boost_system)

install (TARGETS ${prj_name} DESTINATION bin)
