#include <ros/ros.h>
#include <msgs/Motor.h>
#include <sstream>

msgs::Motor control;

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
    ss >> control.left;
    ss.ignore();
    ss >> control.right;

    ROS_INFO("LEFT: %f", control.left);
    ROS_INFO("RIGHT: %f", control.right);

    chatter_pub.publish(control);
    ros::spinOnce();
    loop_rate.sleep();
  }
}