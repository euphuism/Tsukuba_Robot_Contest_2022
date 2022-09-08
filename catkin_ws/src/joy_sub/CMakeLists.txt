cmake_minimum_required(VERSION 3.0.2)
project(joy_sub)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES joy_sub
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(joy_sub_node src/joy_sub_node.cpp)

target_link_libraries(joy_sub_node
  ${catkin_LIBRARIES}
)