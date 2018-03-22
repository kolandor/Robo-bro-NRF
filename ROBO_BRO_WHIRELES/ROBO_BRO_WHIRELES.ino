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

const byte COMMAND_LEFT =  1;
const byte COMMAND_FORWARD =  2;
const byte COMMAND_RIGHT =  3;
const byte COMMAND_BACKWARD =  4;

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
    leftEngine(leftState, leftPower);
    rightEngine(rightState, rightPower);
  }
}

void leftEngine(byte state, byte power) {  
  if(state == 1)  {
    digitalWrite(LEFT_BACKWARD, 0);
    digitalWrite(LEFT_FORWARD, power);
  } else if(state == 0) {
    digitalWrite(LEFT_FORWARD, 0);
    digitalWrite(LEFT_BACKWARD, power);
  }
}

void rightEngine(byte state, byte power) {
  if(state == 1)  {
    digitalWrite(RIGHT_BACKWARD, 0);
    digitalWrite(RIGHT_FORWARD, power);    
  } else if(state == 0) {
    digitalWrite(RIGHT_FORWARD, 0);
    digitalWrite(RIGHT_BACKWARD, power);
  }
}

void MoveCommand(byte &command)
{
  if (command == COMMAND_LEFT)
    {
      printf("COMMAND_LEFT\n");
      MoveEngine(RIGHT_FORWARD, LEFT_BACKWARD);
    }
    else if (command == COMMAND_FORWARD)
    {
      printf("COMMAND_FORWARD\n");
      MoveEngine(RIGHT_FORWARD, LEFT_FORWARD);
    }
    else if (command == COMMAND_RIGHT)
    {
      printf("COMMAND_RIGHT\n");
      MoveEngine(RIGHT_BACKWARD, LEFT_FORWARD);
    }
    else if (command == COMMAND_BACKWARD)
    {
      printf("COMMAND_BACKWARD\n");
      MoveEngine(RIGHT_BACKWARD, LEFT_BACKWARD);
    }
}

void MoveEngine(int rightEngine, int leftEngine)
{
  digitalWrite(rightEngine, HIGH);
  digitalWrite(leftEngine, HIGH);
  delay(10);
  digitalWrite(rightEngine, LOW);
  digitalWrite(leftEngine, LOW);
  delay(10);
}

