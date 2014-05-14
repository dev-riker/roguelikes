set(prj_name ouruo-alpha)
project(${prj_name})
cmake_minimum_required(VERSION 2.8)

include ("/usr/local/include/")
include_directories ("./inc")
include_directories ("/usr/local/include/TCOD")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11")

set(file_srcs src/main.cpp src/actor.cpp src/ai.cpp src/attacker.cpp src/attribute.cpp src/class.cpp src/container.cpp
    src/destructible.cpp src/engine.cpp src/gui.cpp src/map.cpp src/persistent.cpp src/pickable.cpp src/skill.cpp)
set(file_hdrs inc/main.hpp inc/actor.hpp inc/ai.hpp inc/attacker.hpp inc/attribute.hpp inc/class.hpp inc/container.hpp
    inc/destructible.hpp inc/engine.hpp inc/gui.hpp inc/map.hpp inc/persistent.hpp inc/pickable.hpp inc/skill.hpp)

ADD_EXECUTABLE(${prj_name} ${file_srcs} ${file_hdrs})
TARGET_LINK_LIBRARIES(${prj_name} boost_system  tcod tcodxx tcodgui)

install (TARGETS ${prj_name} DESTINATION bin)
