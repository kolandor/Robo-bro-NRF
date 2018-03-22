#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include "SendEngineState.h"


#define FORWARD 2 // A button
#define RIGHT 3 // B button
#define BACK 4 // C button
#define LEFT 5 // D button

const byte COMMAND_LEFT =  1;
const byte COMMAND_FORWARD =  2;
const byte COMMAND_RIGHT =  3;
const byte COMMAND_BACKWARD =  4;

byte oldCommand = 0;



void setup()
{
  Serial.begin(9600);
  printf_begin();//привязка printf() к Serial


  //илициализация управляющих кнопок
  pinMode(FORWARD, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(BACK, INPUT);
  pinMode(LEFT, INPUT);

  initNRF();
}

//engine info
// 0 left direction
// 1 right direction
// 2 left power
// 3 right power


//void sendEngineState(byte leftState, byte leftPower, byte rightState, byte rightPower) {
void sendForward() {
  sendEngineState(1, 255, 1, 255);
}

void sendBack() {
  sendEngineState(0, 255, 0, 255);
}

void sendLeft() {
  sendEngineState(0, 255, 1, 255);
}

void sendRight() {
  sendEngineState(1, 255, 0, 255);
}

void sendStop() {
  sendEngineState(0, 0, 0, 0);
}

bool anyButtonPressed = false;
bool wasPressedOnLastTick = false;
long lastStopTime = 0;
void loop()
{
  anyButtonPressed = false;
  if (!digitalRead(FORWARD))
  {
    if (oldCommand != COMMAND_FORWARD) {
      printf("FORWARD\n");
      sendForward();
    }
    oldCommand = COMMAND_FORWARD;
    anyButtonPressed = true;
  }
  else if (!digitalRead(RIGHT))
  {
    if (oldCommand != COMMAND_RIGHT) {
      printf("RIGHT\n");
      sendRight();
    }
    oldCommand = COMMAND_RIGHT;
    anyButtonPressed = true;
  }
  else if (!digitalRead(BACK))
  {
    if (oldCommand != COMMAND_BACKWARD) {
      printf("BACK\n");
      sendBack();
    }
    oldCommand = COMMAND_BACKWARD;
    anyButtonPressed = true;
  }
  else if (!digitalRead(LEFT))
  {
    if (oldCommand != COMMAND_LEFT) {
      printf("LEFT\n");
      sendLeft();
    }
    oldCommand = COMMAND_LEFT;
    anyButtonPressed = true;
  }
  if (!anyButtonPressed
      && !wasPressedOnLastTick
      && millis() - lastStopTime > 500) {            
    sendStop();
    lastStopTime = millis();
  }

  if (!anyButtonPressed && wasPressedOnLastTick) {    
    sendStop();
    lastStopTime = millis();
  }

  if (!anyButtonPressed) {
    oldCommand = 0;
  }

  wasPressedOnLastTick = anyButtonPressed;
  //delay(10);
}
