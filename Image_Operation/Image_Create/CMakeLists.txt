cmake_minimum_required(VERSION 3.0.0)

SET(OpenCV_DIR /c/opencv-4.1.0/Installation/x64/vc16/staticlib)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#set(CMAKE_BUILD_TYPE Debug)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Images/boy.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/boy.jpg)
MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
ENDMACRO()

add_example(Image_Create)
