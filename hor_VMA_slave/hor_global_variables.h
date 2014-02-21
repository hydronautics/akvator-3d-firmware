//int inCount=0; // количество байт в пришедшей I2C посылке
boolean newMsg; // флаг, отмечающий приход новой посылки 

byte toHorVmaPacket[TO_HORVMA_DATA_SIZE+1];  //пакет данных с ВВМА(контроллер вертикальных вма)плюс 1 байт на контрольную сумму
byte incomingBufferPacket[TO_HORVMA_DATA_SIZE+1]; // буферный пакет

byte fromHorVmaPacket[FROM_HORVMA_DATA_SIZE+1];

boolean yawSauOn=false;
