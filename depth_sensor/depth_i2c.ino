


//----------------------------------------------------

void i2c_init(byte address)
{
  Wire.begin(address);
  //attachInterrupt(INT_NUMBER,main_interrupt,RISING);
  Wire.onReceive(main_interrupt);

  for (int i=0;i<TO_DEPTH_SENSOR_DATA_SIZE+1;i++)
  {
    toDepthSensorPacket[i]=0;
    incomingBufferPacket[i]=0;
  }

  for (int i=0;i<FROM_DEPTH_SENSOR_DATA_SIZE+1;i++)
  {
    fromDepthSensorPacket[i]=0;
  }
}

void buffer_check_and_packet_write(int bytesReceived) //функция проверяет правильность пакета принятого в буфере и либо печатает код ошибки, либо записывает значения из буферного пакета в рабочий пакет
{
  if(bytesReceived!=TO_DEPTH_SENSOR_DATA_SIZE+1)//если кол-во полученных байт не соот-ет размеру пакета
  {
    warning_print(0);//выдать ошибку с кодом 0 - неверный размер
    packet_printing(incomingBufferPacket,TO_DEPTH_SENSOR_DATA_SIZE);// и напечатать пакет
  } 
  else if (!sum_check(incomingBufferPacket,TO_DEPTH_SENSOR_DATA_SIZE))//если же размер верный, то сначала проверяем к.с.
  {
    warning_print(1);//к.с. неверная, так что выдаем ошибку с кодом 1 - "неверная к.с."
    packet_printing(incomingBufferPacket,TO_DEPTH_SENSOR_DATA_SIZE);// и напечатать  пакет
  } 
  else //если же все чики-пуки
  {
    for (int i=0;i<TO_DEPTH_SENSOR_DATA_SIZE+1;i++)
    {
      toDepthSensorPacket[i]=incomingBufferPacket[i]; //записываем данные из буферного пакета в рабочий массив toVerVmaPacket, данные из которого используются для расчетов
    }

    if (inputDebug)
    {
      Serial.print("\nIN ");
      packet_printing(toDepthSensorPacket,TO_DEPTH_SENSOR_DATA_SIZE); // и напечатать  пакет
    }
  }

}


//------------------------------------------------
//void main_interrupt() // howMany - кол-во байт в пришедшей посылке.
void main_interrupt(int howMany) // howMany - кол-во байт в пришедшей посылке.
{
  //interrupts();

  read_buffer(incomingBufferPacket);

  buffer_check_and_packet_write(howMany);

  from_depth_packet_making();
  
  interrupts();
  
  depth_sensor_feedback_sending();
  
  if (depthDebug)
  {
    Serial.print("RawDepth: ");
    Serial.print(rawDepth);
    Serial.print(" Depth: ");
    Serial.println(filteredDepth);

        
  }
  
  if (depthSpeedDebug)
  {
    Serial.print("\tSpeed:\t");
    Serial.println(intDepthVelocity,DEC);
  }
  
  
}






