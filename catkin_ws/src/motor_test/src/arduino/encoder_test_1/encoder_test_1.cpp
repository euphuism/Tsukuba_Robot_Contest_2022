// From https://github.com/kkkenty/robot03/blob/master/src/arduino/motor_controller/motor_control.cpp

#include "encoder_test_1.h"
//------------------------------------------------
//  グローバル変数定義
//------------------------------------------------
const float LIMIT = 0.15, kp = 25.0, ki = 2.0, kd = 0.1; 
const float PERIOD = 10.0; // ms -> 1000/PERIOD[Hz]

// timeの更新が入るため，配列にしてそれぞれのエンコーダ分用意する
unsigned long time_pre[NUM] = {0}, time_now[NUM] = {0};
float SPEED_NOW[NUM] = {0.0}, SPEED_GOAL[NUM] = {0.0}, SPEED_ERROR[NUM] = {0.0};

// 割り込み処理(class宣言外)に使われるため，global変数
volatile long value[NUM] = {0}; // エンコーダの値(割り込みで変化)
volatile int nowSig_A[NUM] = {0}, nowSig_B[NUM] = {0}, oldSig_A[NUM] = {0}, oldSig_B[NUM] = {0}; // A,B相の信号
volatile int nowState[NUM] = {0}, oldState[NUM] = {0}; // A,B相の状態 

//------------------------------------------------
//  概　要：encoderのコンストラクタ
//  引  数：A相ピン番号，B相ピン番号
//------------------------------------------------
encoder::encoder(int A, int B){
  pinMode(A, INPUT);
  pinMode(B, INPUT);
}
//------------------------------------------------
//  概　要：現在の速度の取得
//  引  数：0/1(左/右)
//------------------------------------------------
void encoder::getSPEED(int i){
  time_now[i] = millis();
  value_now = value[i];
  //Serial.println(value_now[i]);
  static float dt = 0.0;
  if((dt = time_now[i] - time_pre[i]) > 50){
    SPEED_NOW[i] = (float)(value_now - value_pre) / dt;
    SPEED_ERROR[i] = SPEED_GOAL[i] - SPEED_NOW[i]; // P項 or 偏差
    //Serial.println(SPEED_NOW[i]);
    time_pre[i] = time_now[i];
    value_pre = value_now;
  }
}
//------------------------------------------------
//  概　要：motorのコンストラクタ
//  引  数：PWMピン番号，DIRピン番号
//------------------------------------------------
motor::motor(int POW, int DIR){
  pinMode(POW, OUTPUT);
  pinMode(DIR, OUTPUT);
}
//------------------------------------------------
//  概　要：PID制御してPWM計算
//  引  数：0/1(左/右)
//------------------------------------------------
void motor::PID(int i){
  SPEED_SUM += (SPEED_ERROR[i] + SPEED_ERROR_PRE) / 2.0; // I項
  SPEED_ACC = (float)(SPEED_NOW[i] - SPEED_PRE) / PERIOD * 1000.0; // D項
  //Serial.println(SPEED_ACC);
  PWM = kp * SPEED_ERROR[i] + ki * SPEED_SUM - kd * SPEED_ACC;
  SPEED_ERROR_PRE = SPEED_ERROR[i];
  SPEED_PRE = SPEED_NOW[i];
}
//------------------------------------------------
//  概　要：台形制御してPWM計算
//  引  数：0/1(左/右)
//------------------------------------------------
void motor::daikei(int i){
  if(SPEED_ERROR[i] > LIMIT){
    PWM += ACC;
  }
  else if(SPEED_ERROR[i] < -LIMIT){
    PWM -= ACC;
  }
  else{ // -LIMIT <= SPEED_ERROR[i] <= LIMIT
  }
}
//------------------------------------------------
//  概　要：モータに出力を送る
//  引  数：PWMピン番号，DIRピン番号
//------------------------------------------------
void motor::Write(int POW, int DIR){
  if(PWM > 255 ) PWM = 255;
  else if(PWM < -255) PWM = -255;
  
  if(PWM >= 0){
    analogWrite(POW, PWM);
    digitalWrite(DIR, LOW);
  }
  else if(PWM < 0){
    analogWrite(POW, -PWM);
    digitalWrite(DIR, HIGH);
  }
  //Serial.println(PWM);
}

//------------------------------------------------
//  概　要：左エンコーダのカウンタ（割り込み）
//------------------------------------------------
void counter0(){
  nowSig_A[0] = digitalRead(PIN_A_LEFT);
  nowSig_B[0] = digitalRead(PIN_B_LEFT);
  
  if(nowSig_A[0] != oldSig_A[0] || nowSig_B[0] != oldSig_B[0]){
    if     (nowSig_A[0] == 0 && nowSig_B[0] == 0) nowState[0] = 0;
    else if(nowSig_A[0] == 1 && nowSig_B[0] == 0) nowState[0] = 1;
    else if(nowSig_A[0] == 1 && nowSig_B[0] == 1) nowState[0] = 2;
    else if(nowSig_A[0] == 0 && nowSig_B[0] == 1) nowState[0] = 3;

    if((oldState[0] == 0 && nowState[0] == 1) || 
       (oldState[0] == 1 && nowState[0] == 2) ||
       (oldState[0] == 2 && nowState[0] == 3) || 
       (oldState[0] == 3 && nowState[0] == 0)){
      value[0]++;
    }
    else if((oldState[0] == 0 && nowState[0] == 3) || 
            (oldState[0] == 3 && nowState[0] == 2) ||
            (oldState[0] == 2 && nowState[0] == 1) || 
            (oldState[0] == 1 && nowState[0] == 0)){
      value[0]--;
    }
    oldSig_A[0] = nowSig_A[0];
    oldSig_B[0] = nowSig_B[0];
    oldState[0] = nowState[0];    
  }
}
//------------------------------------------------
//  概　要：右エンコーダのカウンタ（割り込み）
//------------------------------------------------
void counter1(){
  nowSig_A[1] = digitalRead(PIN_A_RIGHT);
  nowSig_B[1] = digitalRead(PIN_B_RIGHT);
  
  if(nowSig_A[1] != oldSig_A[1] || nowSig_B[1] != oldSig_B[1]){
    if     (nowSig_A[1] == 0 && nowSig_B[1] == 0) nowState[1] = 0;
    else if(nowSig_A[1] == 1 && nowSig_B[1] == 0) nowState[1] = 1;
    else if(nowSig_A[1] == 1 && nowSig_B[1] == 1) nowState[1] = 2;
    else if(nowSig_A[1] == 0 && nowSig_B[1] == 1) nowState[1] = 3;

    if((oldState[1] == 0 && nowState[1] == 1) || 
       (oldState[1] == 1 && nowState[1] == 2) ||
       (oldState[1] == 2 && nowState[1] == 3) || 
       (oldState[1] == 3 && nowState[1] == 0)){
      value[1]++;
    }
    else if((oldState[1] == 0 && nowState[1] == 3) || 
            (oldState[1] == 3 && nowState[1] == 2) ||
            (oldState[1] == 2 && nowState[1] == 1) || 
            (oldState[1] == 1 && nowState[1] == 0)){
      value[1]--;
    }
    oldSig_A[1] = nowSig_A[1];
    oldSig_B[1] = nowSig_B[1];
    oldState[1] = nowState[1];    
  }
}