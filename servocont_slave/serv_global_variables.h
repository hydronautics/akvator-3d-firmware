//int inCount=0; // количество байт в пришедшей I2C посылке
boolean newMsg; // флаг, отмечающий приход новой посылки 

byte toServocontPacket[TO_SERVOCONT_DATA_SIZE+1];  //пакет данных 
byte incomingBufferPacket[TO_SERVOCONT_DATA_SIZE+1]; //буферный пакет

byte fromServocontPacket[FROM_SERVOCONT_DATA_SIZE+1];
