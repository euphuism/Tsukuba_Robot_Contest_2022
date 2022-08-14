#define PIN_PWM_LEFT 10
#define PIN_DIR_LEFT 11
#define PIN_PWM_RIGHT 8
#define PIN_DIR_RIGHT 9

void setup()
{
  pinMode(PIN_PWM_LEFT, OUTPUT);
  pinMode(PIN_DIR_LEFT, OUTPUT);
  pinMode(PIN_PWM_RIGHT, OUTPUT);
  pinMode(PIN_DIR_RIGHT, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  analogWrite(PIN_PWM_LEFT, 124);
  digitalWrite(PIN_DIR_LEFT, HIGH);

  analogWrite(PIN_PWM_RIGHT, 124);
  digitalWrite(PIN_DIR_RIGHT, LOW);
}

