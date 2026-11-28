#define AzimovDriver
#define pastVersion

#include "Driver.h"

Driver driver;

// Настройки
#define SpeedMove 80
#define MaxCorrect 80

//Переменные Насткойки
bool isInvers = false;

// Пины датчиков
const uint8_t dataPins[]  = {A0, A1, A2, A3, A4};
const uint8_t datasCount  = sizeof(dataPins) / sizeof(uint8_t);
const uint8_t lowsSpeed   = SpeedMove - MaxCorrect;
const uint8_t hightsSpeed = SpeedMove + MaxCorrect;
const uint8_t Kp = MaxCorrect / datasCount;  // Коэффициент пропорциональной регуляции смещения

// Состаяния датчиков
bool datas[datasCount];

// Прототипы функций
int8_t countOffset();
void getDatas();
void checkInversion();
void lineGoing();


void setup() {
  for (uint8_t i = 0; i < datasCount; i++) {
    pinMode(dataPins[i], INPUT);
    datas[i] = 0;
  }
  pinMode(3, OUTPUT);   // trigger ультразвукового датчика
  pinMode(2, INPUT);    // echo ультразвукового датчика

  while (measureDistance(3, 2) > 20);
  driver.forward();
  delay(2000);
}

void loop() {
  static uint32_t timer = millis() -1;
  if (timer <= millis()){
    checkInversion();
    lineGoing();
  }
}

// Движение по линии с использованием countOffset()
void lineGoing() {
  int8_t offset = countOffset();

  int correction = offset * Kp;
  int leftSpeed  = SpeedMove - correction;
  int rightSpeed = SpeedMove + correction;

  // Ограничение скорости допустимым диапазоном
  leftSpeed  = constrain(leftSpeed,  -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  driver.go(leftSpeed, rightSpeed);
}

// Определение инверсной линии по двум крайним датчикам:
// если оба крайних видят линию — меняем флаг инверсии
void checkInversion() {
  bool left  = datas[0];
  bool right = datas[datasCount - 1];
  if (left && right) isInvers = !isInvers;
}

int8_t countOffset(){
  static int8_t lastResult = 0;
  bool wasLine = 0;
  uint8_t afterLine = 0;
  uint8_t beforeLine = 0;
  getDatas();
  for(int i = 0; i < datasCount; i++){
    if(datas[i]){
      wasLine = true;
    }else{
      if(wasLine) afterLine++;
      else beforeLine++;
    }
  }
  int8_t result = beforeLine - afterLine;
  if(abs(result) == datasCount) result = (lastResult > 0) ? datasCount : 0 - datasCount;
  lastResult = result;
  return result;
}

void getDatas() {
  for (uint8_t i = 0; i < datasCount; i++) {
    bool raw = analogRead(dataPins[i]) < 500;
    datas[i] = isInvers ? !raw : raw;
  }
}


float measureDistance(int triggerPin, int echoPin) {
  // Подаём короткий импульс на trigger pin (10 мкс)
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Ждём импульс на echo и меряем его длительность в микросекундах
  // Таймаут 25000 мкс (~4.3 м), чтобы не зависнуть при отсутствии эха
  unsigned long duration = pulseIn(echoPin, HIGH, 25000UL);

  // Переводим время в расстояние: скорость звука ~0.0343 см/мкс, делим на 2 — туда-обратно
  constexpr float SPEED_OF_SOUND_CM_PER_US = 0.0343f;
  float distance = SPEED_OF_SOUND_CM_PER_US * duration / 2.0f;

  return distance;
}
