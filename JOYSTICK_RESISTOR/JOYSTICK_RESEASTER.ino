#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SendEngineState.h"

#define LEFT_ANALOG 0
#define RIGHT_ANALOG 1

#define MIDDLE 0
#define TIP 10

int leftPinA = 2;  // номер вывода, подключенный к CLK енкодера
int leftPinB = 4;  // номер вывода контроллера, подключенный к DT енкодера
int leftEncoderPosCount = 0;
int leftPinALast;

int rightPinA = 3;
int rightPinB = 5;
int rightEncoderPosCount = 0;
int rightPinALast;

void setup()
{
  Serial.begin(9600);
  printf_begin();//привязка printf() к Serial

  pinMode (leftPinA, INPUT);
  pinMode (leftPinB, INPUT);

  pinMode (rightPinA, INPUT);
  pinMode (rightPinB, INPUT);
  leftPinALast = digitalRead(leftPinA);
  rightPinALast = digitalRead(rightPinA);
  
  attachInterrupt(0, leftEncoderA, CHANGE); //прерывание по изменению пина №2(для Arduino UNO)
  attachInterrupt(1, rightEncoderA, CHANGE); //прерывание по изменению пина №2(для Arduino UNO)
  initNRF();
}

//void sendEngineState(byte leftState, byte leftPower, byte rightState, byte rightPower) {
void sendForward(int left, int right) {
  //sendEngineState(1, 255, 1, 255);

  byte leftState = left > MIDDLE;
  byte leftPower = abs(left - MIDDLE);

  byte rightState = right > MIDDLE;
  byte rightPower = abs(right - MIDDLE);

  Serial.println(leftState);
  Serial.println(leftPower);
  
  Serial.println(rightState);
  Serial.println(rightPower);
  //sendEngineState(leftState, leftPower, rightState, rightPower);
}

int FixRange(int oldValue, int newValue) {
  return newValue >= -255 && newValue <= 255 ? newValue : oldValue;
}

void rightEncoderA() {
  if (rightPinALast != digitalRead(rightPinA)) {
    if (digitalRead(rightPinA) == HIGH) {
      // проверяем контакт B для определения направления вращения
      if (digitalRead(rightPinB) == LOW) {

        rightEncoderPosCount = FixRange(rightEncoderPosCount, rightEncoderPosCount + TIP);
      }
      else {
        rightEncoderPosCount = FixRange(rightEncoderPosCount, rightEncoderPosCount - TIP);
      }
    }
    else   // следим за переходом high-to-low на выводе A
    {
      // проверяем вывод B для определения направления вращения
      if (digitalRead(rightPinB) == HIGH) {
        rightEncoderPosCount = FixRange(rightEncoderPosCount, rightEncoderPosCount + TIP);
      }
      else {
        rightEncoderPosCount = FixRange(rightEncoderPosCount, rightEncoderPosCount - TIP);
      }
    }
  }
  rightPinALast = digitalRead(rightPinA);
}

void leftEncoderA() {
  // следим за переходом low-to-high на контакте A
  if (leftPinALast != digitalRead(leftPinA)) {
    if (digitalRead(leftPinA) == HIGH) {
      // проверяем контакт B для определения направления вращения
      if (digitalRead(leftPinB) == LOW) {
        leftEncoderPosCount = FixRange(leftEncoderPosCount, leftEncoderPosCount + TIP);
      }
      else {
        leftEncoderPosCount = FixRange(leftEncoderPosCount, leftEncoderPosCount - TIP);
      }
    }
    else   // следим за переходом high-to-low на выводе A
    {
      // проверяем вывод B для определения направления вращения
      if (digitalRead(leftPinB) == HIGH) {
        leftEncoderPosCount = FixRange(leftEncoderPosCount, leftEncoderPosCount + TIP);
      }
      else {
        leftEncoderPosCount = FixRange(leftEncoderPosCount, leftEncoderPosCount - TIP);
      }
    }
  }
  leftPinALast = digitalRead(leftPinA);
}

void loop()
{
  sendForward(leftEncoderPosCount, rightEncoderPosCount);
  delay(500);
}


