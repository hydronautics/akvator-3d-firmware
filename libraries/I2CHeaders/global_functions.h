// Этот файл содержит универсальные функции для работы с пакетами, используемые в различных контроллерах на борту аппарата
// 1. Вспомогательные функции и переменные, используемые в работе программы (РАБОЧИЕ)
// 2. Вспомогательные функции и переменные, используемые только в процессе разработки и отладки канала связи и не используемые в работе конечного продукта (ОТЛАДОЧНЫЕ)
// ============== РАБОЧИЕ ФУНКЦИИ ==========================
//подсчет чексуммы для пакета packet и запись ее в элемент под номером packetSize

byte checksum_calc(const byte packet[], const byte packetSize)
{
  byte checksum=0;
  for (int i=0; i < packetSize; ++i)
  {
    checksum^=packet[i];
  }
  return checksum;
}

//-------------------------------------
//проверка к.с. в пакете packet[] размером packetSize и сравнение ее с контрольной суммой, хранящейся в последней ячейке packet[packetSize]
boolean sum_check(const byte packet[],const byte packetSize) 
{
  byte checksum=0;
  for (int i=0; i < packetSize; ++i)
  {
    checksum^=packet[i];
  };
  if (checksum == packet[packetSize])
    return true;
  else return false;
}
//----------------------------------------------

// Отправка пакета packet[] размером packetSize по адресу address
void packet_sending(const byte address, const byte packet[],const byte packetSize)
{
  Wire.beginTransmission(address);
  Wire.write(packet,packetSize+1); // отправляем packetSize+1 байт, т.к. в последнем байте хранится контрольная сумма
  Wire.endTransmission(); // Непосредственная отправка начинается в этот момент.
}
//-------------------------------------------------------

// Вывод в Serial-порт единой строкой значений элементов пакета packet[] размером packetSize, а также контрольной суммы packet[packetSize]
void packet_printing(const byte packet[],const byte packetSize)  
{
  for (int i=0; i < packetSize; ++i)
  {
    Serial.print(packet[i]);
    Serial.print("\t\t");

  }

  Serial.print("CS: ");

  Serial.println(packet[packetSize],HEX); // Контрольная сумма одним байтом, записанном в 16-ричной системе
}

//------------------------------------------

void warning_print(const boolean errorCode)//0 - wrong size,1 - wrong checksum
{
  Serial.println((errorCode ? "CHECKSUM ERROR! ":"SIZE ERROR! "));
}


//---------------------------------------
void read_buffer(byte packet[])
{
  int j=0;
  while(Wire.available())
  {
    packet[j]=Wire.read();
    j++;
  }
}

// ============= ОТЛАДОЧНЫЕ ФУНКЦИИ ===========================

//// Генерация рандомных значений (в пределах одного байта в элементах пакета packet[]
//void random_packet_generating(byte *packet[], byte packetSize)
//{
//  byte checksum=0;
//  for (int i=0;i < packetSize;i++)
//  {
//    *packet[i]=random(0,255);
//    checksum^=*packet[i];
//  };
//  *packet[packetSize]=checksum;
//}
//
////-------------------------------------------------------



