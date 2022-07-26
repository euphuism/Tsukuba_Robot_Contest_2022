#include <ros.h>
#include <std_msgs/Int64.h>

#define PIN_PWM_LEFT 10
#define PIN_DIR_LEFT 11
#define PIN_PWM_RIGHT 8
#define PIN_DIR_RIGHT 9

ros::NodeHandle nh;
int64_t pwm;

void messangeCb(const std_msgs::Int64& toggle_msg)
{
  pwm = toggle_msg.data;
  
  if(pwm > 255) pwm = 255;
  if(pwm < -255) pwm = -255;
}

ros::Subscriber<std_msgs::Int64> sub("toggle_led", &messangeCb);

void setup()
{
  pinMode(PIN_PWM_LEFT, OUTPUT);  
  pinMode(PIN_DIR_LEFT, OUTPUT);
  pinMode(PIN_PWM_RIGHT, OUTPUT);
  pinMode(PIN_DIR_RIGHT, OUTPUT);

  Serial.begin(57600);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.subscribe(sub);
  pwm = 0;
}
void loop()
{
  nh.spinOnce();

  analogWrite(PIN_PWM_LEFT, pwm);
  digitalWrite(PIN_DIR_LEFT, HIGH);

  analogWrite(PIN_PWM_RIGHT, pwm);
  digitalWrite(PIN_DIR_RIGHT, LOW);

  delay(1);
}
