cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED TFACTORY_VERSION_MAJOR)
  set(TFACTORY_VERSION_MAJOR 0)
endif()

# build option
option(TFACTORY_TIME "show model run time" OFF)
option(TFACTORY_BUILD_SHARED_LIBS "MNN build shared or static lib,'.so/.a'" ON)

message(STATUS "TFactory Build INFO:")
project(TFactory VERSION ${TFACTORY_VERSION_MAJOR} LANGUAGES C CXX)

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -fstrict-aliasing -ffast-math -flax-vector-conversions -pthread -fPIC -fno-stack-protector -Wall")

set(TF_Target "")
file(GLOB TFACTORY_CMAKE ${CMAKE_CURRENT_LIST_DIR}/cmake/dummpy.cpp)
list(APPEND TF_Target ${TFACTORY_CMAKE})

include_directories(${CMAKE_CURRENT_LIST_DIR}/3rd_party/
                    ${CMAKE_CURRENT_LIST_DIR}/3rd_party/imageHelper/
                    ${CMAKE_CURRENT_LIST_DIR}/3rd_party/Opencv/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/tfac/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/config/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/preprocess/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/postprocess/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/tengine/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/pipeline/
                    ${CMAKE_CURRENT_LIST_DIR}/framework/video/
                    ${CMAKE_CURRENT_LIST_DIR}/tools/)

# image
include(${CMAKE_CURRENT_LIST_DIR}/framework/image/CMakeLists.txt)
list(APPEND TF_Target ${TFACTORY_IMAGE_HANDLE})

# config
FILE(GLOB TFACTORY_CONFIG ${CMAKE_CURRENT_LIST_DIR}/framework/config/*)
list(APPEND TF_Target ${TFACTORY_CONFIG})

# preprocess
FILE(GLOB TFACTORY_PREPROCESS ${CMAKE_CURRENT_LIST_DIR}/framework/preprocess/*)
list(APPEND TF_Target ${TFACTORY_PREPROCESS})

# postprocess
FILE(GLOB TFACTORY_POSTPROCESS ${CMAKE_CURRENT_LIST_DIR}/framework/postprocess/*)
list(APPEND TF_Target ${TFACTORY_POSTPROCESS})

# tengine
FILE(GLOB TFACTORY_TENGINE ${CMAKE_CURRENT_LIST_DIR}/framework/tengine/*)
list(APPEND TF_Target ${TFACTORY_TENGINE})

# video
FILE(GLOB TFACTORY_VIDEO ${CMAKE_CURRENT_LIST_DIR}/framework/video/*)
list(APPEND TF_Target ${TFACTORY_VIDEO})

# process
FILE(GLOB TFACTORY_PIPELINE ${CMAKE_CURRENT_LIST_DIR}/framework/pipeline/*)
list(APPEND TF_Target ${TFACTORY_PIPELINE})
# process
FILE(GLOB TFACTORY_PROCESS ${CMAKE_CURRENT_LIST_DIR}/framework/tfac/*)
list(APPEND TF_Target ${TFACTORY_PROCESS})

# tools
FILE(GLOB TFACTORY_TOOLS ${CMAKE_CURRENT_LIST_DIR}/tools/*)
list(APPEND TF_Target ${TFACTORY_TOOLS})

set(distribution_DIR ${CMAKE_CURRENT_LIST_DIR}/libs)
add_library(tenginelite SHARED IMPORTED)
set_target_properties(tenginelite PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libtengine-lite.so)

find_package(OpenCV REQUIRED)

if(TFACTORY_TIME)
  add_definitions(-DTFACTORY_TIME)
endif()

# if (TFACTORY_BUILD_SHARED_LIBS)
  add_library(TFactory SHARED ${TF_Target})
# else()
#   add_library(TFactory STATIC ${TF_Target})
# endif()

target_link_libraries(TFactory tenginelite ${OpenCV_LIBS})