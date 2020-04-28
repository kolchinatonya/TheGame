cmake_minimum_required(VERSION 3.2)
project(TheGame2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES TheGame.cpp Network.cpp Network.h Boost.cpp Boost.h)
add_executable(TheGame2 ${SOURCE_FILES} TheGame.cpp BackGround.cpp BackGround.h Fish.cpp Fish.h Menu.cpp Menu.h)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(TheGame2 ${SFML_LIBRARIES})
endif()