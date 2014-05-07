project(cellular-automata)
cmake_minimum_required(VERSION 2.8)

include ("/usr/local/include/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")

set(file_srcs main.cpp)
set(file_hdrs )

add_definitions(-DLINUX)

ADD_EXECUTABLE(cellular-automata ${file_srcs} ${file_hdrs})
TARGET_LINK_LIBRARIES(cellular-automata boost_system)

install (TARGETS cellular-automata DESTINATION bin)
