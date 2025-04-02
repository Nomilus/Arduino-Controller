#include <PS2X_lib.h>

#define SERIAL Serial

// PWM Pins สำหรับล้อแต่ละตัว
#define FL1 4  // Front Left
#define FL2 5
#define FR1 6  // Front Right
#define FR2 7
#define BL1 8  // Back Left
#define BL2 9
#define BR1 10 // Back Right
#define BR2 11

PS2X ps2x;

int Motor_PWM = 150;
const int PWM_STEP = 10;
const int PWM_MAX = 255;
const int PWM_MIN = 50;

void setMotor(int pin1, int pin2, int pwm1, int pwm2) {
  analogWrite(pin1, pwm1);
  analogWrite(pin2, pwm2);
}

void STOP() {
  SERIAL.println(">> STOP");
  setMotor(FL1, FL2, 0, 0);
  setMotor(FR1, FR2, 0, 0);
  setMotor(BL1, BL2, 0, 0);
  setMotor(BR1, BR2, 0, 0);
}

void Move_Forward() {
  SERIAL.println(">> Move Forward");
  setMotor(FL1, FL2, 0, Motor_PWM);
  setMotor(FR1, FR2, 0, Motor_PWM);
  setMotor(BL1, BL2, 0, Motor_PWM);
  setMotor(BR1, BR2, 0, Motor_PWM);
}

void Move_Backward() {
  SERIAL.println(">> Move Backward");
  setMotor(FL1, FL2, Motor_PWM, 0);
  setMotor(FR1, FR2, Motor_PWM, 0);
  setMotor(BL1, BL2, Motor_PWM, 0);
  setMotor(BR1, BR2, Motor_PWM, 0);
}

void Turn_Left() {
  SERIAL.println(">> Turn Left");
  setMotor(FL1, FL2, Motor_PWM, 0);
  setMotor(FR1, FR2, 0, Motor_PWM);
  setMotor(BL1, BL2, Motor_PWM, 0);
  setMotor(BR1, BR2, 0, Motor_PWM);
}

void Turn_Right() {
  SERIAL.println(">> Turn Right");
  setMotor(FL1, FL2, 0, Motor_PWM);
  setMotor(FR1, FR2, Motor_PWM, 0);
  setMotor(BL1, BL2, 0, Motor_PWM);
  setMotor(BR1, BR2, Motor_PWM, 0);
}

void Strafe_Right() {
  SERIAL.println(">> Strafe Right");
  setMotor(FL1, FL2, Motor_PWM, 0);
  setMotor(FR1, FR2, 0, Motor_PWM);
  setMotor(BL1, BL2, 0, Motor_PWM);
  setMotor(BR1, BR2, Motor_PWM, 0);
}

void Strafe_Left() {
  SERIAL.println(">> Strafe Left");
  setMotor(FL1, FL2, 0, Motor_PWM);
  setMotor(FR1, FR2, Motor_PWM, 0);
  setMotor(BL1, BL2, Motor_PWM, 0);
  setMotor(BR1, BR2, 0, Motor_PWM);
}

void Diagonal_Forward_Left() {
  SERIAL.println(">> Diagonal Forward Left");
  setMotor(FR1, FR2, 0, Motor_PWM);
  setMotor(BR1, BR2, 0, Motor_PWM);
}

void Diagonal_Forward_Right() {
  SERIAL.println(">> Diagonal Forward Right");
  setMotor(FL1, FL2, 0, Motor_PWM);
  setMotor(BL1, BL2, 0, Motor_PWM);
}

void Diagonal_Back_Left() {
  SERIAL.println(">> Diagonal Back Left");
  setMotor(FR1, FR2, Motor_PWM, 0);
  setMotor(BR1, BR2, Motor_PWM, 0);
}

void Diagonal_Back_Right() {
  SERIAL.println(">> Diagonal Back Right");
  setMotor(FL1, FL2, Motor_PWM, 0);
  setMotor(BL1, BL2, Motor_PWM, 0);
}

void checkPS2Control() {
  ps2x.read_gamepad();

  if(ps2x.ButtonPressed(PSB_START) || ps2x.ButtonPressed(PSB_SELECT)) STOP();
  else if(ps2x.Button(PSB_PAD_UP)) Move_Forward();
  else if(ps2x.Button(PSB_PAD_DOWN)) Move_Backward();
  else if(ps2x.Button(PSB_PAD_LEFT)) Turn_Left();
  else if(ps2x.Button(PSB_PAD_RIGHT)) Turn_Right();
  else if(ps2x.Button(PSB_SQUARE)) Diagonal_Forward_Left();
  else if(ps2x.Button(PSB_CIRCLE)) Diagonal_Forward_Right();
  else if(ps2x.Button(PSB_CROSS)) Diagonal_Back_Left();
  else if(ps2x.Button(PSB_TRIANGLE)) Diagonal_Back_Right();
  else if(ps2x.Button(PSB_L1)) {
    Motor_PWM += PWM_STEP;
    if(Motor_PWM > PWM_MAX) Motor_PWM = PWM_MAX;
    SERIAL.print("PWM+ >> "); SERIAL.println(Motor_PWM);
    delay(150);
  }
  else if(ps2x.Button(PSB_L2)) {
    Motor_PWM -= PWM_STEP;
    if(Motor_PWM < PWM_MIN) Motor_PWM = PWM_MIN;
    SERIAL.print("PWM- >> "); SERIAL.println(Motor_PWM);
    delay(150);
  }
}

void IO_init() {
  for (int pin = 4; pin <= 11; pin++) {
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
  }
}

void setup() {
  SERIAL.begin(9600);
  IO_init();

  int error = ps2x.config_gamepad(52, 51, 53, 50, false, false);
  if (error == 0) SERIAL.println("PS2 controller connected.");
  else {
    SERIAL.print("PS2 config error: "); SERIAL.println(error);
    while (1);
  }

  delay(1000);
}

void loop() {
  checkPS2Control();
}
