byte roboBroAddr[6] = "2Node";//адрес робо-бро ему, мы будем отправлять
RF24 radio(9, 10);

void initNRF() {
  //начало работы радио
  radio.begin();
  printf("Is nRF24L01: %d\n\n", radio.isPVariant());//вывод информации о подключении nRF24
  radio.stopListening();//контрольная остановка режима чтения
  radio.openWritingPipe(roboBroAddr);//открытие порта для записи
  radio.setPALevel(RF24_PA_HIGH);//установка уровня питания

  delay(100);//initial time
  radio.printDetails();//вывод отладочной информации
}

void sendEngineState(byte leftState, byte leftPower, byte rightState, byte rightPower) {
  byte engineInfo[] = {
    leftState, leftPower,
    rightState, rightPower
  };
  radio.write(&engineInfo, sizeof(engineInfo));
}

