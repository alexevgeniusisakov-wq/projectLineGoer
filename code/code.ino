#include "Driver.h"

Driver.driver;

// Настройки
#define SpeedMove 100

// Пины датчиков
const uint8_t dataPins[] = {A0, A1, A2, A3, A4};

// Состаяния датчиков
bool datas[5] = {0, 0, 0, 0, 0};

// Переменные
int speedTerning = 0;

void setup() {
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(dataPins[i], INPUT);
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
  go(SpeedMove + speedTerning, SpeedMove - speedTerning);
}

void getDatas() {
  for (uint8_t i = 0; i < 5; i++) {
    datas[i] = analogRead(dataPins[i]) < 500;
  }
}
