/*
*****************МЕЖКОНТРОЛЛЕРНАЯ ШИНА I2C - ОСНОВНАЯ ЧАСТЬ ***************** 
Изначальная среда разработки: Arduino 1.0.2
Платформа - Seeeduino MEGA
Контроллер Atmega1280
Этот файл содержит :
1. Описание обработчика главного прерывавния, т.е. рассылку и прием пакетов.
2. Описание функции инициализации i2c_init()
*/

// =========== ИНИЦИАЛИЗАЦИЯ ===============

void vectornav_init()
{
  IMU.Init(VNAV_BAUDRATE);
  IMU.PrintErrors=false;
}

void i2c_init(byte address) //присоединяемся к шине I2C с адресом address
{
  Serial.println("\nConnection Controller!");
  Wire.begin(address);

  for (int i=0;i<TO_VERVMA_DATA_SIZE+1;i++)
  {
    toVerVmaPacket[i]=0;
  }

  for (int i=0;i<TO_HORVMA_DATA_SIZE+1;i++)
  {
    toHorVmaPacket[i]=0;
  }

  for (int i=0;i<TO_SERVOCONT_DATA_SIZE+1;i++)
  {
    toServocontPacket[i]=0;
  }
  toServocontPacket[TO_SERV_CAM_OFFSET]=128; // среднее положение камеры

  for (int i=0;i<TO_DEPTH_SENSOR_DATA_SIZE+1;i++)
  {
    toDepthSensorPacket[i]=0;
  }

  pinMode(LEAK_PIN,INPUT);
  digitalWrite(LEAK_PIN,LOW);

  attachInterrupt(INT_NUMBER,main_interrupt,RISING);  //внешние прерывания от синхроимпульсов, по восходящему фронту
  Serial.println("\nBEGIN");

  reset_a_slave(HOR_VER_RESET_PIN);

}

// ============= ОБРАБОТЧИК ГЛАВНОГО ПРЕРЫВАНИЯ ===============
void main_interrupt()
{
  //long int start = millis();
  

  
  //digitalWrite(DEBUG_PIN,HIGH);
  interrupts(); //разрешаем обработку других прерываний (Serial,i2c) внутри главного прерывания

  packet_request_and_reading(fromDepthSensorPacket,DEPTH_ADDRESS,FROM_DEPTH_SENSOR_DATA_SIZE,depthDebug);
  
  //noInterrupts();
  

  parse_depth(); //Считываем значения глубины и скорости погружения в переменные feedbackDepth и feeedbackDepthSpeed, для комплексирования

  //complex(); // Комплексирование!

  to_hor_vma_packet_making();
  to_ver_vma_packet_making();
  to_servocont_packet_making();
  toServocontPacket[TO_SERV_ROT_TOOL_LEFT] = control[con_ROTATION_TOOL_L];
  toServocontPacket[TO_SERV_ROT_TOOL_RIGHT] = control[con_ROTATION_TOOL_R];
  toServocontPacket[TO_SERVOCONT_DATA_SIZE] = checksum_calc(toServocontPacket,TO_SERVOCONT_DATA_SIZE);
  to_depth_packet_making();

//interrupts();
  packet_sending(HORVMA_ADDRESS,toHorVmaPacket,TO_HORVMA_DATA_SIZE);
  packet_sending(VERVMA_ADDRESS,toVerVmaPacket,TO_VERVMA_DATA_SIZE);
  packet_sending(SERVOCONT_ADDRESS,toServocontPacket,TO_SERVOCONT_DATA_SIZE);
  packet_sending(DEPTH_ADDRESS,toDepthSensorPacket,TO_DEPTH_SENSOR_DATA_SIZE);
  

  switch (slavePollCount)
  {
  case 3:
    slavePollCount=0; //здесь не нужен Break !!!

  case 0:
    packet_request_and_reading(fromHorVmaPacket,HORVMA_ADDRESS,FROM_HORVMA_DATA_SIZE,horDebug);
    break;

  case 1:
    packet_request_and_reading(fromVerVmaPacket,VERVMA_ADDRESS,FROM_VERVMA_DATA_SIZE,verDebug);
    break;

  case 2:
    packet_request_and_reading(fromServocontPacket,SERVOCONT_ADDRESS,FROM_SERVOCONT_DATA_SIZE,servDebug);
    break;

  default:
    slavePollCount=0;
    break;
  }

//to_hor_vma_packet_making();
//  to_ver_vma_packet_making();
//  to_servocont_packet_making();
//  to_depth_packet_making();
//  
  leakage = map(constrain(LEAK_THRESHOLD - analogRead(LEAK_PIN),0,LEAK_THRESHOLD),0,LEAK_THRESHOLD,0,255);

  report_making();


  read_register(8);  //Yaw, Pitch, Roll
  read_register(19);  // Angular rates
  read_register(18);  // Accelerations




  if (outDebug)
  {
    if (verDebug)
    {
      Serial.print("\n2VER: ");
      packet_printing(toVerVmaPacket,TO_VERVMA_DATA_SIZE);
    }

    if (horDebug)
    {
      Serial.print("\n2HOR: ");
      packet_printing(toHorVmaPacket,TO_HORVMA_DATA_SIZE);
    }

    if (servDebug)
    {
      Serial.print("\n2SERV: ");
      packet_printing(toServocontPacket,TO_SERVOCONT_DATA_SIZE);
    }

    if (depthDebug)
    {
      Serial.print("\n2DEPTH: ");
      packet_printing(toDepthSensorPacket,TO_DEPTH_SENSOR_DATA_SIZE);
    }
  }





  if (IMU.NewYPR)
  {
    if (yprDebug)
    {
      Serial.print(IMU.Yaw,3);
      Serial.print('\t');
      Serial.print(IMU.Pitch,3);
      Serial.print('\t');
      Serial.println(IMU.Roll,3);
    }  
    IMU.NewYPR = 0;
  } 

  else {  
    if (vectorNavErrors) {
      Serial.println("NoYPRData :(");
    }
  }

  if (IMU.NewAng)
  {
    if (gyroDebug)
    {
      Serial.print(IMU.Angular[X],4);
      Serial.print('\t');
      Serial.print(IMU.Angular[Y],4);
      Serial.print('\t');
      Serial.println(IMU.Angular[Z],4);
    }
    IMU.NewAng = 0;
  }

  else {  
    if (vectorNavErrors) {
      Serial.println("NoAngularData :(");
    }
  }

  if (IMU.NewAccel)
  {
    if (accelDebug)
    {
      Serial.print(IMU.Acceleration[0],3);
      Serial.print('\t');
      Serial.print(IMU.Acceleration[1],3);
      Serial.print('\t');
      Serial.println(IMU.Acceleration[2],3);
    }
    IMU.NewAccel = 0;
  }

  else {  
    if (vectorNavErrors) {
      Serial.println("NoAccelData :(");
    }
  }

  IMU.Read();

  

  if (      depthInputDebug )
  {

    Serial.print("\nDepth: ");
    Serial.print(feedbackDepth,DEC);
    Serial.print("\tDepthSpeed: ");
    Serial.println(feedbackDepthSpeed,DEC);
  }

  slavePollCount++;

  //Serial.println(leakage,DEC);
  digitalWrite(DEBUG_PIN,LOW);
  //Serial.println(millis()-start);
}



















