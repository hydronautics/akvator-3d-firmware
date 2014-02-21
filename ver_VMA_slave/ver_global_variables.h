//int inCount=0; // количество байт в пришедшей I2C посылке
boolean newMsg; // флаг, отмечающий приход новой посылки 


byte toVerVmaPacket[TO_VERVMA_DATA_SIZE+1];  //пакет данных с ВВМА(контроллер вертикальных вма)плюс 1 байт на контрольную сумму
byte incomingBufferPacket[TO_VERVMA_DATA_SIZE+1]; // буферный пакет

byte fromVerVmaPacket[FROM_VERVMA_DATA_SIZE+1];

boolean pitchRollSauOn;
boolean depthSauOn;
