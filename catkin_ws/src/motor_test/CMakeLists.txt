cmake_minimum_required(VERSION 3.0.2)
project(motor_test)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES moter_test
  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(publisher src/publisher.cpp)
add_executable(subscriber src/subscriber.cpp)
add_executable(pub_power src/arduino/encoder_test/pub_power.cpp)

# add_dependencies()

target_link_libraries(publisher
  ${catkin_LIBRARIES}
)
target_link_libraries(subscriber
  ${catkin_LIBRARIES}
)
target_link_libraries(pub_power
  ${catkin_LIBRARIES})