cmake_minimum_required(VERSION 3.0.2)
project(msgs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

add_message_files(
  FILES
  Motor.msg
  BallCDN.msg
  # MotorLED.msg
  # PID.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES msgs
  CATKIN_DEPENDS roscpp rospy std_msgs tf message_runtime
#  DEPENDS system_lib
)

# include_directories(
# # include
#  ${catkin_INCLUDE_DIRS}
# )
