#include "Driver.h"
#include <stdint.h>

// Конструктор по умолчанию
Driver::Driver() {
  #ifdef ConstructorInit
  ConstructorInit()
  #endif
}

// Пользовательский конструктор
Driver::Driver(uint8_t typeTmp, uint8_t LMotorDirectionPin, uint8_t LMotorSpeedPin, uint8_t RMotorDirectionPin, uint8_t RMotorSpeedPin) {
  driverType = typeTmp;

  leftDirPin1 = LMotorDirectionPin;
  rightDirPin1 = RMotorDirectionPin;
  if(driverType == 0){
    leftSpeedPin = LMotorSpeedPin;
    rightSpeedPin = RMotorSpeedPin;
  }else{
    leftDirPin2 = LMotorSpeedPin;
    rightDirPin2 = RMotorSpeedPin;
  }

  StdPinModesInit()
}

Driver::Driver(uint8_t LMotorSpeedPin, uint8_t LMotorDirectionPin, uint8_t LMotorDirectionPin2, uint8_t RMotorSpeedPin, uint8_t RMotorDirectionPin, uint8_t RMotorDirectionPin2) {
  driverType = 2;
  deadZone = StandartDeadZone;

  leftSpeedPin = LMotorSpeedPin;
  rightSpeedPin = RMotorSpeedPin;
  leftDirPin1 = LMotorDirectionPin;
  rightDirPin1 = RMotorDirectionPin;
  leftDirPin2 = LMotorDirectionPin2;
  rightDirPin2 = RMotorDirectionPin2;

  pinMode(leftDirPin1, OUTPUT);
  pinMode(rightDirPin1, OUTPUT);
  pinMode(leftDirPin2, OUTPUT);
  pinMode(rightDirPin2, OUTPUT);
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
}


// Методы настройки
void Driver::setDeadZone(uint8_t MinMotorSpeedTmp) {
  deadZone = MinMotorSpeedTmp;
}


// Методы управления движением
//Полный мост вперёд
void Driver::forward(uint8_t Speed) {
  drive(Speed, Speed);
}

//Полный мост назад
void Driver::backward(int Speed) {
  drive(0 - Speed, 0 - Speed);
}

//Полный мост на поворот
void Driver::turnInPlace(bool Direction, uint8_t Speed) {
  if (Direction == 0)drive(0 - Speed,Speed);
  else drive( Speed,0 - Speed);
}  

//Тормаз!!!
void Driver::Stop() {
  setMotorSpeeds(0, 0);
}

//Главные методы движения
void Driver::drive(int leftSpeed, int rightSpeed) {
  #ifdef smooth_changing
  targetLeftSpeed = leftSpeed;
  targetRightSpeed = rightSpeed;
  Update();
  setMotorSpeeds(LeftSpeed, RightSpeed);
  #else
  setMotorSpeeds(leftSpeed, rightSpeed);
  #endif
}

void Driver::setMotorSpeeds(int leftSpeed, int rightSpeed) {
  bool M1Direction;
  bool M2Direction;
  
  if(abs(leftSpeed) < deadZone)leftSpeed = 0;
  if(abs(rightSpeed) < deadZone)rightSpeed = 0;

  if(leftSpeed > 0) M1Direction = 0;
  else M1Direction = 1;

  if(rightSpeed > 0) M2Direction = 0;
  else M2Direction = 1;

  if(driverType != 1){
    if (driverType == 0) {
      digitalWrite(leftDirPin1, M1Direction);
      digitalWrite(rightDirPin1, M2Direction);
    } else if (driverType == 2){
      digitalWrite(leftDirPin1, M1Direction);
      digitalWrite(leftDirPin2, !M1Direction);
      
      digitalWrite(rightDirPin1, M2Direction);
      digitalWrite(rightDirPin2, !M2Direction);
    }
    analogWrite(leftSpeedPin, abs(leftSpeed));
    analogWrite(rightSpeedPin, abs(rightSpeed));
  } else {
    activateDirectionPin(leftDirPin1, leftDirPin2, M1Direction, leftSpeed);
    activateDirectionPin(rightDirPin1, rightDirPin2, M2Direction, rightSpeed);
  }
}  

void Driver::activateDirectionPin(uint8_t pin1, uint8_t pin2, uint8_t Direction, int Speed) {
  digitalWrite(pin1, 0);
  digitalWrite(pin2, 0);
  analogWrite((Direction == 1) ? pin1 : pin2, abs(Speed));
}
