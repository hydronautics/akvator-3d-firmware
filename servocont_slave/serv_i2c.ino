
//----------------------------------------------------

void i2c_init(byte address)
{
  Serial.println("\nSERVOCONT CONTROLLOR, 15.06.2013");
  Wire.begin(address);
  //attachInterrupt(INT_NUMBER,main_interrupt,RISING);
  Wire.onReceive(main_interrupt);
  
  for (int i=0;i<TO_SERVOCONT_DATA_SIZE+1;i++)
  {
    toServocontPacket[i]=0;
    incomingBufferPacket[i]=0;
  }
  toServocontPacket[TO_SERV_CAM_OFFSET]=128;
}

//----------------------------------------------------

void buffer_check_and_packet_write(int bytesReceived)
{
  if(bytesReceived!=TO_SERVOCONT_DATA_SIZE+1)//если кол-во полученных байт не соот-ет размеру пакета
  {
    warning_print(0);//выдать ошибку с кодом 0 - неверный размер

    packet_printing(incomingBufferPacket,TO_SERVOCONT_DATA_SIZE);// и напечатать ебаный пакет
  } 
  else if (!sum_check(incomingBufferPacket,TO_SERVOCONT_DATA_SIZE))//если же размер верный, то сначала проверяем к.с.
  {
    warning_print(1);//к.с. неверная, так что выдаем ошибку с кодом 1 - "неверная к.с."
    packet_printing(incomingBufferPacket,TO_SERVOCONT_DATA_SIZE);// и напечатать ебаный пакет
  } 
  else //если же все чики-пуки
  {
    for (int i=0;i<TO_SERVOCONT_DATA_SIZE+1;i++)
      toServocontPacket[i]=incomingBufferPacket[i];

    if (inputDebug)
    {
      Serial.print("\nSERV IN: ");//помечаем пакет как верный и пересылаем далее на оборудование
      packet_printing(toServocontPacket,TO_SERVOCONT_DATA_SIZE); // и напечатать пакет
    }
  }
}

void main_interrupt(int howMany)
{
  
  interrupts();

  tools_output(toServocontPacket[TO_SERV_LIGHT_OFFSET],toServocontPacket[TO_SERV_CAM_OFFSET],toServocontPacket[TO_SERV_GRAB_OFFSET],toServocontPacket[TO_SERV_SHOULDER_OFFSET],toServocontPacket[TO_SERV_ELBOW_OFFSET],toServocontPacket[TO_SERV_WRIST_OFFSET],toServocontPacket[TO_SERV_ROT_TOOL_LEFT],toServocontPacket[TO_SERV_ROT_TOOL_RIGHT] );
  //tools_output(toServocontPacket[TO_SERV_LIGHT_OFFSET],toServocontPacket[TO_SERV_CAM_OFFSET],toServocontPacket[TO_SERV_GRAB_OFFSET],manOneSpeed,manTwoSpeed,manThreeSpeed);
  
  //man_test_control();

  read_buffer(incomingBufferPacket);

  buffer_check_and_packet_write(howMany);
  
  from_servocont_packet_making();

}







