#include "encoder_test_1.h"
#include <ros.h>
#include <msgs/Motor.h>

encoder enc_left(PIN_A_LEFT, PIN_B_LEFT);
encoder enc_right(PIN_A_RIGHT, PIN_B_RIGHT);
motor   mot_left(PIN_PWM_LEFT, PIN_DIR_LEFT);
motor   mot_right(PIN_PWM_RIGHT, PIN_DIR_RIGHT);

unsigned long protect_time_pre = 0, protect_time_now = 0;

ros::NodeHandle nh;

void joyCb(const msgs::Motor& get_msg){
  SPEED_GOAL[0] = get_msg.left;
  SPEED_GOAL[1] = get_msg.right;
}

ros::Subscriber<msgs::Motor> sub("power", &joyCb);

void setup() {
  Serial.begin(57600);
  attachInterrupt(3, counter0, CHANGE);
  attachInterrupt(2, counter0, CHANGE);
  attachInterrupt(0, counter1, CHANGE);
  attachInterrupt(1, counter1, CHANGE);
  nh.initNode();
  nh.subscribe(sub);
  //初期設定
  time_pre[0] = millis();
  time_pre[1] = millis(); 
  protect_time_pre = millis();
}

void loop() {
  
  nh.spinOnce();

  // 現在の速度の取得
  enc_left.getSPEED(0);
  enc_right.getSPEED(1);

  // 制御でPWM計算
  mot_left.daikei(0);
  mot_right.daikei(1);

  // PWMを0にする
  protect_time_now = millis();
  if(SPEED_GOAL[0] == 0.0 && SPEED_GOAL[1] == 0.0){
    if(protect_time_now - protect_time_pre > 5000){
      mot_left.PWM = 0; mot_right.PWM = 0;
    }
    else{
      // 目標速度0のときにカウント開始
    }
  }
  else{
    protect_time_pre = protect_time_now; 
  }
  
  // 速度の出力
  mot_left.Write(PIN_PWM_LEFT, PIN_DIR_LEFT);
  mot_right.Write(PIN_PWM_RIGHT, PIN_DIR_RIGHT);
  
  delay(PERIOD);
}
