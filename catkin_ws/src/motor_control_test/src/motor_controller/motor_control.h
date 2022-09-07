#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
//------------------------------------------------
#include "Arduino.h"
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
// #defineでは全ての変数の値が定義した値に変化するため，注意
#define PIN_A_LEFT 20 //-> 3
#define PIN_B_LEFT 21 //-> 2
#define PIN_PWM_LEFT 10
#define PIN_DIR_LEFT 11
#define PIN_A_RIGHT 2 //-> 0
#define PIN_B_RIGHT 3 //-> 1
#define PIN_PWM_RIGHT 8
#define PIN_DIR_RIGHT 9
#define NUM 2 // モータの個数 0がleft, 1がright
//------------------------------------------------
//  extern宣言
//------------------------------------------------
extern unsigned long time_pre[NUM];
extern float SPEED_GOAL[NUM];
extern const float PERIOD; // ms -> 1000/PERIOD[Hz]
//------------------------------------------------
//  クラス定義(Class definition)
//------------------------------------------------
class encoder{
  private:
    long value_pre = 0, value_now = 0;
  public:
    encoder(int A, int B);
    void getSPEED(int i); // 現在の速度の取得
};

class motor{
  private:
    int ACC = 1;
    float SPEED_SUM = 0.0, SPEED_ACC = 0.0, SPEED_ERROR_PRE = 0.0, SPEED_PRE = 0.0;
  public:
    int PWM = 0;
    motor(int POW, int DIR);
    void PID(int i); // PID制御でPWM計算
    void daikei(int i); // 台形制御でPWM計算
    void Write(int POW, int DIR); // 速度の出力
};
//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
void counter0();
void counter1();
//------------------------------------------------
#endif