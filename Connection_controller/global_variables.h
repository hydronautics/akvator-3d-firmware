// ========== РАБОЧИЕ ПЕРЕМЕННЫЕ (ГЛОБАЛЬНЫЕ) =====================

byte leakage=0;

//пакеты данных, отправляемые на слейвы
byte toHorVmaPacket[TO_HORVMA_DATA_SIZE+1];  //пакет данных с ГВМА (контроллер горизонтальных ВМА)плюс 1 байт на контрольную сумму
byte toVerVmaPacket[TO_VERVMA_DATA_SIZE+1];  //пакет данных с ВВМА(контроллер вертикальных вма)
byte toServocontPacket[TO_SERVOCONT_DATA_SIZE+1];  //пакет данных с контроллера приспособлений
byte toDepthSensorPacket[TO_DEPTH_SENSOR_DATA_SIZE+1]; //пакет данных с  датчика глубины

//пакеты данных, принятые со слейвов
byte fromHorVmaPacket[FROM_HORVMA_DATA_SIZE+1];  //пакет данных с ВВМА(контроллер вертикальных вма)плюс 1 байт на контрольную сумму
byte fromVerVmaPacket[FROM_VERVMA_DATA_SIZE+1];  //пакет данных с ГВМА (контроллер горизонтальных ВМА)
byte fromServocontPacket[FROM_SERVOCONT_DATA_SIZE+1];  //пакет данных с контроллера сервомашинок
byte fromDepthSensorPacket[FROM_DEPTH_SENSOR_DATA_SIZE+1]; //пакет данных с  датчика глубины


byte slavePollCount=0; //номер слейва, опрашиваемого на текущем такте
boolean inMainInterrupt=0;  //флаг факта входа в главное прерывание
