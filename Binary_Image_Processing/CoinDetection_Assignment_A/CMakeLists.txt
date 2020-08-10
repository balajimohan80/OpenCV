cmake_minimum_required(VERSION 3.0.0)

SET(OpenCV_DIR /c/opencv-4.1.0/Installation/x64/vc16/staticlib)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#set(CMAKE_BUILD_TYPE Debug)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Images/CoinsB.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Images/CoinsA.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message("CMAKE_CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR       : ${CMAKE_CURRENT_BINARY_DIR}")

# add_custom_target("IMAGE")
# add_custom_command(
#	TARGET "IMAGE" 
#	PRE_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../Images/muskaaa.jpg ${CMAKE_CURRENT_BINARY_DIR}
#)

MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
ENDMACRO()

add_example( Coin_Detection_Assignments_A )
