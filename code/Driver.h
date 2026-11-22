#ifndef Driver_h
#define Driver_h

#include <Arduino.h>

#pragma region Defines

#define StdPinModesInit() \
  deadZone = StandartDeadZone;    \
  pinMode(leftSpeedPin, OUTPUT);  \
  pinMode(rightSpeedPin, OUTPUT);  \
  pinMode(leftDirPin1, OUTPUT);  \
  pinMode(rightDirPin1, OUTPUT);
#ifdef AzimovDriver
#define ConstructorInit()   leftSpeedPin = 10;    \
  rightSpeedPin = 11;  \
  leftDirPin1 = 12;  \
  rightDirPin1 = 13;  \
  leftDirPin2 = 0;  \
  rightDirPin2 = 0;  \
  type = 0;    \
  StdPinModesInit()
#elif defined(AmperkaDriver)
#define ConstructorInit()   leftSpeedPin = 6;    \
  rightSpeedPin = 5;   \
  leftDirPin1 = 7;  \
  rightDirPin1 = 4;  \
  leftDirPin2 = 0;  \
  rightDirPin2 = 0;  \
  type = 0;    \
  StdPinModesInit()
#else
#define offConstructure
#endif

#pragma region Class
// Класс драйвера двигателей
class Driver {
  public:
    // Конструкторы
    #ifndef offConstructure 
      Driver();
    #endif

    Driver(uint8_t typeTmp, uint8_t M1Pin1, uint8_t M1Pin2, uint8_t M2Pin1, uint8_t M2Pin2);
    Driver(uint8_t LMotorSpeedPin, uint8_t LMotorDirectionPin, uint8_t LMotorDirectionPin2, uint8_t RMotorSpeedPin, uint8_t RMotorDirectionPin, uint8_t RMotorDirectionPin2);
    
    // Методы настройки
    void setDeadZone(uint8_t MinMotorSpeedTmp);

    // Методы управления движением
    void forward(uint8_t Speed);
    void backward(int Speed);
    void turnInPlace(bool Direction, uint8_t Speed);
    void Stop();
    void drive(int leftSpeed, int rightSpeed);
    void setMotorSpeeds(int leftSpeed, int rightSpeed);

    #ifdef smooth_changing
      void Driver::Update() {
        if(millis() > nextUpdateTime){
          if(LeftSpeed > targetLeftSpeed)LeftSpeed -= rampStep;
          if(LeftSpeed < targetLeftSpeed)LeftSpeed += rampStep;
          
          if(RightSpeed > targetRightSpeed)RightSpeed -= rampStep;
          if(RightSpeed < targetRightSpeed)RightSpeed += rampStep;
          
          setMotorSpeeds(LeftSpeed, RightSpeed);

          nextUpdateTime = millis() + rampDelayMs;
        }
      }
      void Driver::setRampRate(uint8_t Step, uint8_t delayMs){
        rampStep = Step;
        rampDelayMs = delayMs;
      }
    #endif

    #ifdef pastVersion
      [[deprecated("Use forward() instead")]]
      void forvard(uint8_t s) { forward(s); }

      [[deprecated("Use backward() instead")]]
      void backvard(uint8_t s) { backward(s); }

      [[deprecated("Use turnInPlace() instead")]]
      void around(bool d, uint8_t s) { turnInPlace(d, s); }

      [[deprecated("Use drive() instead")]]
      void go(int s1, int s2) { drive(s1, s2); }

      [[deprecated("Use setMotorSpeeds() instead")]]
      void Move(int s1, int s2) { setMotorSpeeds(s1, s2); }

      #ifdef smooth_changing
        [[deprecated("Use setDeadZone() instead")]]
        void setStartSpeed(uint8_t s) { setDeadZone(s); }

        [[deprecated("Use update() instead")]]
        void Tick() { Update(); }
      #endif
    #endif
  private:
    // Запрет конструктора
    #ifdef offConstructure
      Driver();
    #endif


    // Внутрение методы
    void activateDirectionPin(uint8_t pin1, uint8_t pin2, uint8_t Direction, int Speed);

    // Пины управления движением
    uint8_t leftSpeedPin;
    uint8_t rightSpeedPin;

    uint8_t leftDirPin1;
    uint8_t rightDirPin1;

    uint8_t leftDirPin2;
    uint8_t rightDirPin2;

    // Настройки
    uint8_t driverType;
    uint8_t deadZone;
    uint8_t StandartDeadZone = 70;

    #ifdef smooth_changing
      uint32_t nextUpdateTime = 0;
      uint8_t rampStep = 0;
      uint8_t rampDelayMs = 0;

      int LeftSpeed = 0;
      int RightSpeed = 0;
      int targetLeftSpeed = 0;
      int targetRightSpeed = 0;
    #endif
};

#pragma region End
#endif
