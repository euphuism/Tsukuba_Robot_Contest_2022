#include <ros/ros.h>
#include <sensor_msgs/Joy.h>

// define the allocation of sticks and buttons
#define LEFT_AXES_LR 0;
#define LEFT_AXES_UD 1;
#define RIGHT_AXES_LR 2;
#define RIGHT_AXES_UD 3;

void joy_callback(const sensor_msgs::Joy &joy_msg)
{
  // 処理内容を記述
  ROS_INFO("axes[0]:%f", joy_msg.axes[0]);   // スティック0の状態を表示 (-1 ～ 1)
  ROS_INFO("Button[0]:%d", joy_msg.buttons[0]);  // ボタン0の状態を表示 (0 or 1)
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "joy_sub_node");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("joy", 10, joy_callback);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}