


//----------------------------------------------------

void i2c_init(byte address)
{
  Serial.println("\nHORVMA CONTROLLOR");
  Wire.begin(address);
  //attachInterrupt(INT_NUMBER,main_interrupt,RISING);
  Wire.onReceive(main_interrupt);

  for (int i=0;i<TO_HORVMA_DATA_SIZE+1;i++)
  {
    toHorVmaPacket[i]=0;
    incomingBufferPacket[i]=0;
  }

  for (int i=0;i<FROM_HORVMA_DATA_SIZE+1;i++)
    fromHorVmaPacket[i]=0;
}

void buffer_check_and_packet_write(int bytesReceived) //функция проверяет правильность пакета принятого в буфере и либо печатает код ошибки, либо записывает значения из буферного пакета в рабочий пакет
{
  if(bytesReceived!=TO_HORVMA_DATA_SIZE+1)//если кол-во полученных байт не соот-ет размеру пакета
  {
    warning_print(0);//выдать ошибку с кодом 0 - неверный размер
    packet_printing(incomingBufferPacket,TO_HORVMA_DATA_SIZE);// и напечатать пакет
  } 
  else if (!sum_check(incomingBufferPacket,TO_HORVMA_DATA_SIZE))//если же размер верный, то сначала проверяем к.с.
  {
    warning_print(1);//к.с. неверная, так что выдаем ошибку с кодом 1 - "неверная к.с."
    packet_printing(incomingBufferPacket,TO_HORVMA_DATA_SIZE);// и напечатать  пакет
  } 
  else //если же все чики-пуки
  {
    for (int i=0;i<TO_HORVMA_DATA_SIZE+1;i++)
    {
      toHorVmaPacket[i]=incomingBufferPacket[i]; //записываем данные из буферного пакета в рабочий массив toVerVmaPacket, данные из которого используются для расчетов
      newMsg=true;
    }


  }

}


//------------------------------------------------
void main_interrupt(int howMany) // howMany - кол-во байт в пришедшей посылке.
{
  //interrupts();

  read_buffer(incomingBufferPacket);

  buffer_check_and_packet_write(howMany);

  parse_feedback_data();
  
  yawSauOn=toHorVmaPacket[TO_HOR_SAU_BYTE]/255;
  
  if (yawSauOn)
    tau();
  else
    {
      //error_speed=0;
      timeDelay=0;
      //testValue=0;
      //OldJoy=0;
    timeDelayGlobal=0;
    }
  distribute_horisontal_output_char_X_Y_Yaw((char)toHorVmaPacket[TO_HOR_X_SPEED_OFFSET],(char)toHorVmaPacket[TO_HOR_Y_SPEED_OFFSET],(char)toHorVmaPacket[TO_HOR_YAW_SPEED_OFFSET]);
  
  pwm_output(); // выводим на драйвера ВМА значения, рассчитанные на предыдущем цикле

  from_horvma_packet_making();
  
  interrupts();

  if (newMsg)
  {

    if (inputDebug)
    {
      Serial.print("\nIN: ");
      packet_printing(toHorVmaPacket,TO_HORVMA_DATA_SIZE); // и напечатать  пакет
    }

    if (vnavDebug)
    {
      Serial.print("\nKurs:\t");
      Serial.print(Kurs);
      Serial.print("\tKursSpeed:\t");
      Serial.println(KursSpeed,4);
    }
  }

  newMsg=false;
}






