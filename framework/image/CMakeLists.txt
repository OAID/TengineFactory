include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

file (GLOB_RECURSE ImageHandleSrc "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
set  (TFACTORY_IMAGE_HANDLE)
list (APPEND TFACTORY_IMAGE_HANDLE ${ImageHandleSrc})