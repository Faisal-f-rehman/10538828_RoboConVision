cmake_minimum_required(VERSION 3.5)

project(RoboConVision LANGUAGES CXX)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RoboConVision main.cpp vision.cpp guicomms.cpp)

target_link_libraries(RoboConVision ${OpenCV_LIBS} )
