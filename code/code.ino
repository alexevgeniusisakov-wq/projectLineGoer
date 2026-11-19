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
bool isLineDetected();
void lineGoing();

// Настройки движения по линии
#define Kp 30            // Коэффициент пропорциональной регуляции смещения
#define SearchSpeed 60   // Скорость поиска линии при её потере

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
  int8_t offset = countOffset();

  if (offset == 0 && !isLineDetected()) {
    // Линия не найдена ни одним датчиком — ищем её
    driver.go(-SearchSpeed, SearchSpeed);
    return;
  }

  int correction = offset * Kp;
  int leftSpeed  = SpeedMove - correction;
  int rightSpeed = SpeedMove + correction;

  // Ограничение скорости допустимым диапазоном
  leftSpeed  = constrain(leftSpeed,  -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  driver.go(leftSpeed, rightSpeed);
}

// Есть ли линия хотя бы под одним датчиком
bool isLineDetected() {
  for (uint8_t i = 0; i < pinsCount; i++) {
    if (datas[i] != isInvers) return true;
  }
  return false;
}

int8_t countOffset(){
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
  return beforeLine - afterLine;
}

void getDatas() {
  for (uint8_t i = 0; i < pinsCount; i++) {
    datas[i] = analogRead(dataPins[i]) < 500;
  }
}
