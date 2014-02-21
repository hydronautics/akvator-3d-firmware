// ========== РАБОЧИЕ ПЕРЕМЕННЫЕ (ГЛОБАЛЬНЫЕ) =====================

//управляющие сигналы с берега, значения от 0 до 255
byte moveArray[MOVE_ARRAY_SIZE];
byte camServArray[CAMSERV_ARRAY_SIZE];
byte manArray[MAN_ARRAY_SIZE];
byte toolsServArray[TOOLSSERV_ARRAY_SIZE];

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

// ================ ОТЛАДОЧНЫЕ ПЕРЕМЕННЫЕ (ГЛОБАЛЬНЫЕ) ====================
// Массивы, содержащие пакеты с фиксированными данными, сгенерированными специальными ф-ями типа fixed_hor_vma_packet_array_generating
// Каждый массив имеет размер FIXED_PACKETS_AMOUNT. Каждый элемент массива - это пакет для рассылки одного из типовых размеров (напр. TO_HORVMA_DATA_SIZE+1)
byte (toHorFixPackArray[FIXED_PACKETS_AMOUNT*2])[TO_HORVMA_DATA_SIZE+1];  //массив фиксированных пакетов для отправки на КГВМА
byte (toVerFixPackArray[FIXED_PACKETS_AMOUNT])[TO_VERVMA_DATA_SIZE+1];  //массив фиксированных пакетов для отправки на КВВМА
byte (toServFixPackArray[FIXED_PACKETS_AMOUNT])[TO_SERVOCONT_DATA_SIZE+1];//массив фиксировованных пакетов для отправки на контроллер сервомашинок
byte (toDepthSensorFixPackArray[FIXED_PACKETS_AMOUNT])[TO_DEPTH_SENSOR_DATA_SIZE+1];// массив фиксированных пакетов для отправки на контроллер датчика глубины

int fixedPacketCount=0; //номер текущего фиксированного пакета в массиве фиксированных пакетов
