


//----------------------------------------------------

void i2c_init(byte address)
{
  Serial.println("\nVERVMA CONTROLLOR 31.05.13");
  Wire.begin(address);
  //attachInterrupt(INT_NUMBER,main_interrupt,RISING);
  Wire.onReceive(main_interrupt);

  for (int i=0;i<TO_VERVMA_DATA_SIZE+1;i++)
  {
    toVerVmaPacket[i]=0;
    incomingBufferPacket[i]=0;
  }

  for (int i=0;i<FROM_VERVMA_DATA_SIZE+1;i++)
    fromVerVmaPacket[i]=0;
}





void buffer_check_and_packet_write(int bytesReceived) //функция проверяет правильность пакета принятого в буфере и либо печатает код ошибки, либо записывает значения из буферного пакета в рабочий пакет
{
  if(bytesReceived!=TO_VERVMA_DATA_SIZE+1)//если кол-во полученных байт не соот-ет размеру пакета
  {
    warning_print(0);//выдать ошибку с кодом 0 - неверный размер
    packet_printing(incomingBufferPacket,TO_VERVMA_DATA_SIZE);// и напечатать пакет
  } 
  else if (!sum_check(incomingBufferPacket,TO_VERVMA_DATA_SIZE))//если же размер верный, то сначала проверяем к.с.
  {
    warning_print(1);//к.с. неверная, так что выдаем ошибку с кодом 1 - "неверная к.с."
    packet_printing(incomingBufferPacket,TO_VERVMA_DATA_SIZE);// и напечатать  пакет
  } 
  else //если же все чики-пуки
  {
    for (int i=0;i<TO_VERVMA_DATA_SIZE+1;i++)
    {
      toVerVmaPacket[i]=incomingBufferPacket[i]; //записываем данные из буферного пакета в рабочий массив toVerVmaPacket, данные из которого используются для расчетов
      newMsg=true; //поднимаем флаг прихода новой посылки    
    }
  }

}


//------------------------------------------------
// пришла посылка по i2c, в этом обработчике прерывания будет не только чтение пришедшей посылки, но и обработка этих данных, т.е. прерывание будет "долгое"
void main_interrupt(int howMany) // howMany - кол-во байт в пришедшей посылке.
{
  //interrupts(); //разрешаем прерывания внутри этого прерывания, иначе не будет не работать практически ничего
  

  
  


  read_buffer(incomingBufferPacket); //читаем пришедшую посылку в массив incomingBufferPacket

  buffer_check_and_packet_write(howMany);  // проверяем правильность данных, лежащих в incomingBufferPacket и записываем их в массив toVerVmaPacket

  parse_feedback_data();
  
  Depth=word(toVerVmaPacket[TO_VER_COMPLEX_DEPTH_HIGH],toVerVmaPacket[TO_VER_COMPLEX_DEPTH_LOW]);
  DepthSpeed= word(toVerVmaPacket[TO_VER_COMPLEX_DEPTH_SPEED_HIGH],toVerVmaPacket[TO_VER_COMPLEX_DEPTH_SPEED_LOW]);
  
 // Serial.print("Depth: ");
  //Serial.println(Depth,DEC);
  
  
  if (toVerVmaPacket[TO_VER_SAU_BYTE]>=170)
    pitchRollSauOn=true;
    else
    pitchRollSauOn=false;
  
  if (toVerVmaPacket[TO_VER_SAU_BYTE]==85 || toVerVmaPacket[TO_VER_SAU_BYTE]==255)
    depthSauOn=true;
    else 
    depthSauOn=false;
    
    
  if (pitchRollSauOn)
  {
  tau_kren();
  tau_diff();
  }
  else
  {
  timeDelayGlobal=0;
  error_speed_kren =0;
  error_speed_diff=0;
  
  }
  
  
  if (depthSauOn)
  {
  tau_glub();
  }
  else
  {
    timeDelay=0;
      //testValue=0;
      //OldJoy=0;
    timeDelayGlobal=0;
  }
  
  //Serial.println(pitchRollSauOn);
  distribute_vertical_output_char_Z_Roll_Pitch((char)toVerVmaPacket[TO_VER_Z_SPEED_OFFSET],0,0); // рассчитываем значения для выставления на ВМА
  
  pwm_output(); // выводим на драйвера ВМА значения, рассчитанные на предыдущем цикле

  from_vervma_packet_making(); //составляем ответный пакет для отправки на контроллер связи
  

  
  interrupts();
  
  
  
  if (newMsg)
  {
    if (inputDebug)
    {
      Serial.print("\nIN ");
      packet_printing(toVerVmaPacket,TO_VERVMA_DATA_SIZE); // и напечатать  пакет
    }

    if (vnavDebug)

    {
      Serial.print("\nPitch: ");
      Serial.print(Different);

      Serial.print("\tRoll: ");
      Serial.print(Kren);
      
      Serial.print("\tPitch Speed: ");
      Serial.print(DifferentSpeed,4);

      Serial.print("\tRoll Speed: ");
      Serial.println(KrenSpeed,4);
    }
    
    if (depthDebug==1)
  {
      //Serial.print(testValue);
    Serial.print(" glub => "); 
    Serial.print(Depth,DEC);
     Serial.print(" DepthSpeed => "); 
    Serial.print(DepthSpeed,DEC); 
    Serial.print(" Joy => "); 
    Serial.print(Joy,DEC); 
  //  Serial.print(" Old_Joy => "); 
  //  Serial.print(OldJoy,DEC); 
       Serial.print(" signal => "); 
      Serial.println(error_speed_glub);
  }
  }
  newMsg = false;
}














