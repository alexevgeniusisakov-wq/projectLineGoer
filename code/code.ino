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

// Переменные
int speedTerning = 0;

void setup() {
  for (uint8_t i = 0; i < pinsCount; i++) {
    pinMode(dataPins[i], INPUT);
    datas[i] = 0;
  }
}

void loop() {
  getDatas();
  if(datas[2] == 1){
    speedTerning = 0;
  }else if(datas[1] == 1){
    speedTerning = -80;
  }else if(datas[3] == 1){
    speedTerning = 80;
  }else if(datas[0] == 1){
    speedTerning = -160;
  }else if(datas[4] == 1){
    speedTerning = 160;
  }else{
    // нужен таймер
    // а потом поиск линии
  }
  driver.go(SpeedMove + speedTerning, SpeedMove - speedTerning);
}

int8_t countOffset(){
  int8_t result = 0;
  bool wasLine = 0;
  uint8_t afterLine = 0;
  uint8_t beforeLine = 0;
  getDatas();
  for(int i = 0; i < pinsCount; i++){
    if(datas[i] != isInversiv){
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
