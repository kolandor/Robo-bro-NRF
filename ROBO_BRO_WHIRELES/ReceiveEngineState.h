
RF24 radio(9, 10);
byte roboBroAddr[6] = "2Node";//адрес робо-бро, он будет принимать данные

void initNRF() {
  //начало работы радио
  radio.begin();//инициализация
  printf("Is nRF24L01: %d\n\n", radio.isPVariant());//вывод информации о подключении nRF24
  radio.openReadingPipe(0, roboBroAddr);//открытие порта для чтения
  radio.setPALevel(RF24_PA_HIGH);//установка уровня питания
  radio.startListening();//начало прослушивания
  radio.printDetails();//вывод отладочной информации
}

bool receiveEngineState(byte &leftState, byte &leftPower, byte &rightState, byte &rightPower) { 
  if (radio.available()) {
    byte engineInfo[4];
    radio.read(&engineInfo, sizeof(engineInfo));

    leftState = engineInfo[0];
    leftPower = engineInfo[1];
    rightState = engineInfo[2];
    rightPower = engineInfo[3];
    return true;
  }
  return false;
}

