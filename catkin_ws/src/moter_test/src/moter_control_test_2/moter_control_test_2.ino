#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;

void messangeCb(const std_msgs::Int64& toggle_msg)
{
  
  digitalWrite(13, HIGH-digitalRead(13));
}

ros::Subscriber<std_msgs::Int64> sub("toggle_led", &messangeCb);

void setup()
{
    pinMode(13, OUTPUT);
    nh.initNode();
    nh.subscribe(sub);
}

void loop()
{
    nh.spinOnce();
    delay(1);
}