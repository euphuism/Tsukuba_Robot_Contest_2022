#include <ros/ros.h>
#include <msgs/Motor.h>
#include <sstream>

msgs::Motor control;
float mot_left = 0.0, mot_right = 0.0;

int main(int argv,char** argc){

  ros::init(argv,argc,"pub_power");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  ros::Publisher chatter_pub = nh.advertise<msgs::Motor>("chatter", 10);
  std::string msg_chatter = "HalloWorld";
  pnh.getParam("power", msg_chatter);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    std::stringstream ss;
    ss << msg_chatter;
    ss >> mot_left;
    ss.ignore();
    ss >> mot_right;

    control.left = mot_left;
    control.right = mot_right;

    ROS_INFO("LEFT: %f", control.left);
    ROS_INFO("RIGHT: %f", control.right);

    chatter_pub.publish(control);
    ros::spinOnce();
    loop_rate.sleep();
  }
}