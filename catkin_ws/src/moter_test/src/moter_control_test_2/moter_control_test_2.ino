#include <ros.h>
#include <std_msgs/Empty.h>

#define PIN_PWM_LEFT 10
#define PIN_DIR_LEFT 11
#define PIN_PWM_RIGHT 8
#define PIN_DIR_RIGHT 9

ros::NodeHandle nh;
int64_t pwm1;

void messangeCb(const std_msgs::Empty& toggle_msg)
{
  pwm1 = toggle_msg.toInt();
}

ros::Subscriber<std_msgs::Empty&> sub("toggle_led", &messangeCb);

void setup()
{
  pinMode(PIN_PWM_LEFT, OUTPUT);  
  pinMode(PIN_DIR_LEFT, OUTPUT);
  pinMode(PIN_PWM_RIGHT, OUTPUT);
  pinMode(PIN_DIR_RIGHT, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();

  analogWrite(PIN_PWM_LEFT, pwm1);
  digitalWrite(PIN_DIR_LEFT, HIGH);

  analogWrite(PIN_PWM_RIGHT, pwm1);
  digitalWrite(PIN_DIR_RIGHT, LOW);

  delay(1);
}
