#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "ReceiveEngineState.h"

/*распиновка беловола
int RIGHTWHEEL_BACKWARD = 5;
int RIGHTWHEEL_FORWARD = 4;
int LEFTWHEEL_FORWARD = 3;
int LEFTWHEEL_BACKWARD = 2;*/

/* Предыдущая распиновка до добавления ENA ENB
#define LEFT_FORWARD 3
#define LEFT_BACKWARD 2
#define RIGHT_FORWARD 4
#define RIGHT_BACKWARD 5*/

#define LEFT_FORWARD 5
#define LEFT_BACKWARD 4
#define RIGHT_FORWARD 7
#define RIGHT_BACKWARD 8

#define POWER_RIGHT 6
#define POWER_LEFT 3

void setup()
{
  Serial.begin(9600);
  printf_begin();//привязка printf() к Serial

  //илициализация управляющих ножек
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);

  initNRF();
}

void loop()
{
  moveBot();
}

void moveBot() {
  byte leftState;
  byte leftPower;
  byte rightState;
  byte rightPower;
  bool isAvailable = receiveEngineState(leftState, leftPower, rightState, rightPower);

  if(isAvailable) {
//    Serial.write(leftState);
//    Serial.write(leftPower);
//    Serial.write(rightState);
//    Serial.write(rightPower);
//    Serial.write("\n");
//    
    Move(leftState, leftPower, rightState, rightPower);
  }
}

void Move(byte &leftState, byte &leftPower, byte &rightState, byte &rightPower)
{
    analogWrite(POWER_LEFT, leftPower);
    digitalWrite(LEFT_FORWARD, leftState);   
    digitalWrite(LEFT_BACKWARD, !leftState); 

    analogWrite(POWER_RIGHT, rightPower);
    digitalWrite(RIGHT_FORWARD, rightState);
    digitalWrite(RIGHT_BACKWARD, !rightState);
}



