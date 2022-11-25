cmake_minimum_required(VERSION 3.0.2)
project(detect_ball)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
  tf
  msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(detect_ball_node   src/detect_ball_node.cpp)

target_link_libraries(detect_ball_node
  ${catkin_LIBRARIES}
)
