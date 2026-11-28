#define AzimovDriver
#define pastVersion

#include "Driver.h"

Driver driver;

// Настройки
#define SpeedMove 100

//Переменные Насткойки
bool isInvers = false;

// Пины датчиков
const uint8_t dataPins[] = {A0, A1, A2, A3, A4};
const uint8_t pinsCount = sizeof(dataPins) / sizeof(uint8_t);

// Состаяния датчиков
bool datas[pinsCount];

// Прототипы функций
int8_t countOffset();
void getDatas();
void lineGoing();

// Настройки движения по линии
#define Kp 30            // Коэффициент пропорциональной регуляции смещения

void setup() {
  for (uint8_t i = 0; i < pinsCount; i++) {
    pinMode(dataPins[i], INPUT);
    datas[i] = 0;
  }
}

void loop() {
  lineGoing();
}

// Движение по линии с использованием countOffset()
void lineGoing() {
  static uint32_t timer = millis() -1;
  if (timer <= millis()){
    int8_t offset = countOffset();

    int correction = offset * Kp;
    int leftSpeed  = SpeedMove - correction;
    int rightSpeed = SpeedMove + correction;

    // Ограничение скорости допустимым диапазоном
    leftSpeed  = constrain(leftSpeed,  -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    driver.go(leftSpeed, rightSpeed);
    timer = millis() + 100;
  }
}

int8_t countOffset(){
  static int8_t lastResult = 0;
  bool wasLine = 0;
  uint8_t afterLine = 0;
  uint8_t beforeLine = 0;
  getDatas();
  for(int i = 0; i < pinsCount; i++){
    if(datas[i] != isInvers){
      wasLine = true;
    }else{
      if(wasLine) afterLine++;
      else beforeLine++;
    }
  }
  int8_t result = beforeLine - afterLine;
  if(abc(result) == pinsCount) result = result * (lastResult / abs(lastResult));
  return result;
}

void getDatas() {
  for (uint8_t i = 0; i < pinsCount; i++) {
    datas[i] = analogRead(dataPins[i]) < 500;
  }
}
