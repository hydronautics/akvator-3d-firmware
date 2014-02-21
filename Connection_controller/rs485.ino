//*********************КАНАЛ СВЯЗИ RS485*********************//

//Формат входящего кадра:
//[0-start][zeroCorr][request][][][][sumSt][sumMl]
#include <Arduino.h> 


//ИНИЦИАЛИЗАЦИЯ СВЯЗИ С ПК
void rs485_init() 
{
  RS485_SERIAL.begin(RS485_BAUD);
  pinMode(RE_DE_PIN,OUTPUT);
  rs485_receiver_enable();
}

//ПОДПРОГРАММА ПРИЁМА ДАННЫХ С ПК
void rs485_receive () 
{
  int byteCounter = 0; 
  byte incomingMsg[ RS485_INCOMING_MSG_LENGTH ];
  while (RS485_SERIAL.available()) {
    //просматриваем входящий байт
    byte nextByte = RS485_SERIAL.peek();
    //выводим байт в Serial монитор
    if (RS485_PRINT_ON_RECEIVE == 1) {
      Serial.print(nextByte, DEC);
      Serial.print("  ");
    }
    //проверяем, не стартовый ли байт к нам пришёл
    if (nextByte == 0) {
      //проверяем число пакетов во входном буфере
      int packetsInBuffer = rs485_get_number_of_packets();
      //если пакетов 0, выходим из подпрограммы
      if (packetsInBuffer == 0) {
        break;
      } else {
        //если пакет 1, считываем байт и устанавоиваем счётчик в 1
        if (packetsInBuffer == 1) {
          incomingMsg[0] = RS485_SERIAL.read();
          byteCounter = 1;
        //если пакетов больше одного, читаем байты один за другим,
        //таким образом очищая входной буффер
        } else {
          RS485_SERIAL.read();
          byteCounter = 0; 
        }
      } 
    //если входящий байт - не стартовый
    } else { 
      //проверяем, начался ли пакет. Если нет - считываем байт,
      //таким образом удаляя его из буффера
      if (byteCounter == 0) {
        RS485_SERIAL.read();
      //если пакет начался, записываем входящий байт в массив
      //и увеличиваем счетчик на 1
      } else {
          incomingMsg[byteCounter] = RS485_SERIAL.read();
          byteCounter++;     
      }
    }
    //если приём завершен
    if (byteCounter == RS485_INCOMING_MSG_LENGTH) {
      //выполняем обратное замещение нулей
      //www.grov.wikidot.com/zero_correction/
      boolean zeroCorrectionDone = rs485_zero_decoding(incomingMsg);
      if (zeroCorrectionDone == false) {
        errors++;  
      } else {
        //проверяем контрольную сумму
        boolean msgCorrect = rs485_check_csum(incomingMsg);
        //если к. с. совпала, переписыываем сообщение в массив
        //управляющих переменных
        if (msgCorrect == true) {
          rs485_update_controls(incomingMsg);
          correctData++; 
          if (RS485_PRINT_CONTROL_DATA == 1) {
            rs485_print_control();
          }
        } else {
          //иначе отмечаем факт ошибки при приёме
          errors++;
        }
      }
    }
  }
  //новая строка вывода в Serial monitor
  if (RS485_PRINT_ON_RECEIVE == 1) {
    Serial.println();
  }
  lastReceiveTime = millis();
  return;
}


//ПОДПРОГРАММА ОЦЕНКИ КОЛИЧЕСТВА ПАКЕТОВ,
//СОДЕРЖАЩИХСЯ В БУФЕРЕ ПРИЁМА RS485
int rs485_get_number_of_packets() 
{
  int res = floor(RS485_SERIAL.available() / RS485_INCOMING_MSG_LENGTH);
  return res;
}


//ПОДПРОГРАММА ОБРАТНОЙ ЗАМЕНЫ НУЛЕЙ
//www.grov.wikidot.com/zero_correction/
boolean rs485_zero_decoding(byte msg[])
{
  int length = RS485_INCOMING_MSG_LENGTH;
  int zeroByte = 1;
  while (msg[zeroByte] != 255) {
    if (msg[zeroByte] > (length - 1))
      return false;
    byte nextZeroByte = msg[zeroByte];
    msg[zeroByte] = 0;
    zeroByte = nextZeroByte;
  }   
  msg[zeroByte] = 0;
  return true;
}


//ПОДПРОГРАММА РАСЧЕТА КОНТРОЛЬНОЙ СУММЫ
boolean rs485_check_csum(byte msg[])
{
  int length = RS485_INCOMING_MSG_LENGTH;
  int csumMainByte = length - 2;
  int csumLowByte = length - 1;
  int csumIn = int(msg[csumMainByte])<<8;
  csumIn += int(msg[csumLowByte]);
  int csumRes = 0;
  for (int i = 1; i < csumMainByte; i++) {
    unsigned int absValue = abs(char(msg[i]));
    csumRes += absValue;
  }
  if (csumIn == csumRes) {
    return true;
  } else {
    return false;
  }
}


//КОПИРОВАНИЕ ДАННЫХ В МАССИВ CONTROLS[]
void rs485_update_controls(byte msg[])
{
  for (int i = 0; i <= CONTROL_BYTES_AMOUNT - 1; i++) {
    control[i] = msg[i + 2];  
  }
}


//ПОДПРОГРАММА ОТПРАВКИ ДАННЫХ НА ПК
void rs485_reply() {
  //включаем передатчик имс МАХ485
  rs485_driver_enable();
  //инициализируем пакет данных  
  byte sendingMsg [RS485_SENDING_MSG_LENGTH];
  //формируем пакет для отправки
  rs485_form_message(sendingMsg);
  //отправляем данные по UART
  rs485_send_to_uart(sendingMsg);
  //включаем приёмник имс МАХ485
  rs485_receiver_enable();  
}


//ПОДПРОГРАММА ВКЛЮЧЕНИЯ ПРИЁМНИКА ИМС МАХ485
void rs485_receiver_enable() {
  digitalWrite(RE_DE_PIN, LOW);
}


//ПОДПРОГРАММА ВКЛЮЧЕНИЯ ПЕРЕДАТЧИКА ИМС МАХ485
void rs485_driver_enable() {
  digitalWrite(RE_DE_PIN, HIGH);
}


//ПОДПРОГРАММА, ФОРМИРУЮЩАЯ СООБЩЕНИЕ ДЛЯ ОПРАВКИ НА ПК
void rs485_form_message (byte msg[]) {
  byte length = RS485_SENDING_MSG_LENGTH;
  long checkSum = 0;
  msg[0] = 0;
  //обновление данных и расчёт контрольной суммы
  for (byte i = 0; i <= REPORT_BYTES_AMOUNT - 1;  i++) {
    msg[i + 2] = report[i];
    checkSum += abs(char(report[i]));
  }
  msg[length - 2] = (checkSum >> 8) & B11111111;
  msg[length - 1] = checkSum & B11111111;
  byte zeroByte = 1;
  for (byte i = 2; i <= (length - 1); i++)
    if (msg[i]==0) {
      msg[zeroByte] = i;
      zeroByte = i;
    }
  msg[zeroByte] = 255;
}


//ПОДПРОГРАММА ПРЯМОЙ ПЕРЕДАЧИ ДАННЫХ В UART (ВЕРСИЯ ДЛЯ РАБОТЫ С BYTE)
void rs485_send_to_uart(byte msg[]) {
  byte length = RS485_SENDING_MSG_LENGTH;
  for (byte i = 0; i <= length - 1; i++) {
    loop_until_bit_is_set(UCSR2A, UDRE2);
    UDR2 = msg[i];
  }
  loop_until_bit_is_set(UCSR2A, UDRE2);
  unsigned long bufferEmptyTime = micros();
  //время, необходимое приёмнику для считывания последнего байта
  while (micros() - bufferEmptyTime < (1000000 * 8 * 1/float(RS485_BAUD))) { ; }
}


//ПОДПРОГРАММА ПРЯМОЙ ПЕРЕДАЧИ ДАННЫХ В UART (ВЕРСИЯ ДЛЯ РАБОТЫ С CHAR)
void rs485_send_to_uart(char msg[]) {
  byte length = strlen(msg);
  Serial.print("len: ");
  Serial.print(length);
  Serial.print("  msg: ");
  Serial.println(msg);
  for (byte i = 0; i <= length - 1; i++) {
    loop_until_bit_is_set(UCSR2A, UDRE2);
    UDR2 = msg[i];
  }
  loop_until_bit_is_set(UCSR2A, UDRE2);
  unsigned long bufferEmptyTime = micros();
  //время, необходимое приёмнику для считывания последнего байта
  while (micros() - bufferEmptyTime < (1000000 * 8 * 1/float(RS485_BAUD))) { ; }
}
