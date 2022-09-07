#include <ros/ros.h>
#include <std_msgs/String.h>

int main(int argv,char** argc){

   ros::init(argv,argc,"publisher_node");
   ros::NodeHandle nh;
   ros::Rate loop_rate(10);
   ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 10);

   while (ros::ok())
   {
     std_msgs::String msg;
     std::stringstream ss;
     ss << "hello world ";
     msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());

    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();

   }
}