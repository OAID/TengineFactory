cmake_minimum_required(VERSION 3.10)

project(${BUILD_FILE})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fdata-sections -fPIE -fstrict-aliasing -ffunction-sections -ffast-math -flax-vector-conversions -O3 -Wall -fsigned-char -fno-stack-protector -fopenmp -g")
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src/include/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/include/Opencv)

set(distribution_DIR ${CMAKE_CURRENT_LIST_DIR}/libs)
add_library(factory SHARED IMPORTED)
set_target_properties(factory PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libTFactory.so)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR}/bin/)

file(GLOB srcpp src/*.cpp)
add_executable(${BUILD_FILE} ${srcpp})

target_link_libraries(
    ${BUILD_FILE}
    factory
    ${OpenCV_LIBS})