cmake_minimum_required(VERSION 3.15)
project(geojson_spatial_analyzer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(geojson-spatial-analyzer src/main.cpp src/bbox_utils.cpp)
add_executable(geojson-spatial-analyzer-api src/api_main.cpp src/bbox_utils.cpp)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(geojson-spatial-analyzer PRIVATE cpr::cpr)
target_link_libraries(geojson-spatial-analyzer-api PRIVATE cpr::cpr)

if(WIN32)
    target_link_libraries(geojson-spatial-analyzer-api PRIVATE ws2_32 mswsock)
endif()