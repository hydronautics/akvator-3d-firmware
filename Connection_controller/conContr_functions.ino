// В этом файле находятся функции, используемые только в коде контроллера связи

// Составление пакета для отправки на контроллер вертикалки

#define ACCEL_RANGE 30.0

#define ANGULAR_RANGE 3.0

void to_ver_vma_packet_making()
{
  toVerVmaPacket[TO_VER_Z_SPEED_OFFSET]=control[con_DEPTH];

  int temp=float_to_int_180(IMU.Pitch);

  toVerVmaPacket[TO_VER_FEEDBACK_PITCH_HIGH]=highByte(temp);
  toVerVmaPacket[TO_VER_FEEDBACK_PITCH_LOW]=lowByte(temp);

  temp = angular_to_int(IMU.Angular[Y]);
  toVerVmaPacket[TO_VER_FEEDBACK_PITCH_SPEED_HIGH]= highByte(temp);
  toVerVmaPacket[TO_VER_FEEDBACK_PITCH_SPEED_LOW]= lowByte(temp);

  temp=float_to_int_180(IMU.Roll);

  toVerVmaPacket[TO_VER_FEEDBACK_ROLL_HIGH]=highByte(temp);
  toVerVmaPacket[TO_VER_FEEDBACK_ROLL_LOW]=lowByte(temp);

  temp = angular_to_int(IMU.Angular[X]);
  toVerVmaPacket[TO_VER_FEEDBACK_ROLL_SPEED_HIGH]= highByte(temp);
  toVerVmaPacket[TO_VER_FEEDBACK_ROLL_SPEED_LOW]= lowByte(temp);
  

  toVerVmaPacket[TO_VER_COMPLEX_DEPTH_HIGH] = highByte(feedbackDepth);
  toVerVmaPacket[TO_VER_COMPLEX_DEPTH_LOW] = lowByte(feedbackDepth);
  toVerVmaPacket[TO_VER_COMPLEX_DEPTH_SPEED_HIGH] = highByte(feedbackDepthSpeed);
  toVerVmaPacket[TO_VER_COMPLEX_DEPTH_SPEED_LOW] = lowByte(feedbackDepthSpeed);
  
  toVerVmaPacket[TO_VER_SAU_BYTE] = (control[con_SAU_SWITCH] & ((1<<con_ROLL_PITCH_BIT)|(1<<con_DEPTH_BIT)))*85; //85 это 255/3. 

  //потому что у нас максимум четыре состояния:
  // 00,01,10,11
  // Сау выключены => 0
  // Включена САУ по глубине ==> 85
  // Включена САУ по углам крена и дифферента ==> 170
  //включены обе САУ => 255

  toVerVmaPacket[TO_VER_DEPTH_SHIFT] = control[con_DEPTH_SHIFT];
   
  control[con_DEPTH_SHIFT] = 0; // чтобы не слать один пришедший пакет несколько раз
  
  toVerVmaPacket[TO_VERVMA_DATA_SIZE] = checksum_calc(toVerVmaPacket,TO_VERVMA_DATA_SIZE);

}

//----------------------------------------
// Составление пакета для отправки на контроллер горизонталки
void to_hor_vma_packet_making()
{
  toHorVmaPacket[TO_HOR_X_SPEED_OFFSET]=control[con_MARCH];
  toHorVmaPacket[TO_HOR_Y_SPEED_OFFSET]=control[con_LAG];
  toHorVmaPacket[TO_HOR_YAW_SPEED_OFFSET]=control[con_YAW];

  int temp=float_to_int_180(IMU.Yaw);

  toHorVmaPacket[TO_HOR_FEEDBACK_YAW_HIGH]=highByte(temp);
  toHorVmaPacket[TO_HOR_FEEDBACK_YAW_LOW]=lowByte(temp);

  temp = angular_to_int(IMU.Angular[Z]);
  toHorVmaPacket[TO_HOR_FEEDBACK_YAW_SPEED_HIGH]= highByte(temp);
  toHorVmaPacket[TO_HOR_FEEDBACK_YAW_SPEED_LOW]= lowByte(temp);
  
  toHorVmaPacket[TO_HOR_SAU_BYTE]= (control[con_SAU_SWITCH] >> con_YAW_BIT)*255;

  toHorVmaPacket[TO_HORVMA_DATA_SIZE] = checksum_calc(toHorVmaPacket,TO_HORVMA_DATA_SIZE);
}
// Составление пакета для отправки на контроллер приспособлений

void to_servocont_packet_making()
{
  toServocontPacket[TO_SERV_LIGHT_OFFSET]=control[con_LIGHT];
  toServocontPacket[TO_SERV_CAM_OFFSET]=control[con_CAMERA_POS];
  toServocontPacket[TO_SERV_GRAB_OFFSET]=control[con_MAN_GRAB];
  
  toServocontPacket[TO_SERV_SHOULDER_OFFSET] = control[con_MAN_ANGLE_1];
  toServocontPacket[TO_SERV_ELBOW_OFFSET] = control[con_MAN_ANGLE_2];
  toServocontPacket[TO_SERV_WRIST_OFFSET] = control[con_MAN_ANGLE_3];
  
  //toServocontPacket[TO_SERV_ROT_TOOL_LEFT] = control[con_ROTATION_TOOL_L];
  //toServocontPacket[TO_SERV_ROT_TOOL_RIGHT] = control[con_ROTATION_TOOL_R];


  
  //toServocontPacket[TO_SERVOCONT_DATA_SIZE] = checksum_calc(toServocontPacket,TO_SERVOCONT_DATA_SIZE);
}

// Составление пакета для отправки на датчик давления

void to_depth_packet_making()
{
  toDepthSensorPacket[TO_DEPTH_ACCEL_OFFSET]=65;

  toDepthSensorPacket[TO_DEPTH_SENSOR_DATA_SIZE] = checksum_calc(toDepthSensorPacket,TO_DEPTH_SENSOR_DATA_SIZE);
}

void packet_request_and_reading(byte packet[],int address,int howManyBytes,boolean debug)
{

  byte bufferPacket[howManyBytes+1];
  Wire.requestFrom(address,howManyBytes+1);
  
  
  if (byte receivedAmount=Wire.available())
  {
    if(receivedAmount==(howManyBytes+1))
    {
      read_buffer(bufferPacket);
      if (sum_check(bufferPacket,howManyBytes))
      {

        for (int i=0;i<howManyBytes+1;i++)
          packet[i]=bufferPacket[i];

        if (debug&&feedbackDebug)
        {
          Serial.print("\nFeedback from ");
          Serial.print(address,DEC);
          Serial.print(": ");
          packet_printing(packet,howManyBytes);
        }
      }

      else
      {
        if (dataErrors) {
          Serial.print("\nWrong checksum from ");
          Serial.print(address,DEC);
          Serial.print(": ");
          packet_printing(bufferPacket,howManyBytes);
        }
      }

    }
    else
    {
      if (dataErrors) {
        Serial.print('\n');
        Serial.print(receivedAmount);
        Serial.print(" IS A WRONG SIZE FROM");
        Serial.println(address,DEC);
      }
    }
  }
  else
  {
    if (dataErrors) {
      Serial.print("\nNo response from: ");
      Serial.println(address,DEC);
    }
  }

}

//========================================

void reset_a_slave(byte reset_pin)
{
  pinMode(reset_pin,OUTPUT);
  digitalWrite(reset_pin,LOW);
  delay(5);
  digitalWrite(reset_pin,HIGH);
  pinMode(reset_pin,INPUT);
}

//Сборка пакета для отправки на берег

void report_making()
{
  report[rep_HLF_CURRENT] = fromHorVmaPacket[F_HOR_HLF_CURRENT_OFFSET];
  report[rep_HLB_CURRENT] = fromHorVmaPacket[F_HOR_HLB_CURRENT_OFFSET];
  report[rep_HRB_CURRENT] = fromHorVmaPacket[F_HOR_HRB_CURRENT_OFFSET];
  report[rep_HRF_CURRENT] = fromHorVmaPacket[F_HOR_HRF_CURRENT_OFFSET];

  report[rep_VLF_CURRENT] = fromVerVmaPacket[F_VER_VLF_CURRENT_OFFSET];
  report[rep_VLB_CURRENT] = fromVerVmaPacket[F_VER_VLB_CURRENT_OFFSET];
  report[rep_VRB_CURRENT] = fromVerVmaPacket[F_VER_VRB_CURRENT_OFFSET];
  report[rep_VRF_CURRENT] = fromVerVmaPacket[F_VER_VRF_CURRENT_OFFSET];

  report[rep_HLF_FORCE] = fromHorVmaPacket[F_HOR_HLF_FORCE_OFFSET];
  report[rep_HLB_FORCE] = fromHorVmaPacket[F_HOR_HLB_FORCE_OFFSET];
  report[rep_HRB_FORCE] = fromHorVmaPacket[F_HOR_HRB_FORCE_OFFSET];
  report[rep_HRF_FORCE] = fromHorVmaPacket[F_HOR_HRF_FORCE_OFFSET];

  report[rep_VLF_FORCE] = fromVerVmaPacket[F_VER_VLF_FORCE_OFFSET]; 
  report[rep_VLB_FORCE] = fromVerVmaPacket[F_VER_VLB_FORCE_OFFSET];
  report[rep_VRB_FORCE] = fromVerVmaPacket[F_VER_VRB_FORCE_OFFSET];
  report[rep_VRF_FORCE] = fromVerVmaPacket[F_VER_VRF_FORCE_OFFSET];
  
  report[rep_DEPTH_LOW]   = fromDepthSensorPacket[FROM_DEPTH_LOW];
  report[rep_DEPTH_HIGH] = fromDepthSensorPacket[FROM_DEPTH_HIGH];

  report[rep_YAW_LOW]    = toHorVmaPacket[TO_HOR_FEEDBACK_YAW_LOW];
  report[rep_YAW_HIGH]   = toHorVmaPacket[TO_HOR_FEEDBACK_YAW_HIGH];
  report[rep_PITCH_LOW]  = toVerVmaPacket[TO_VER_FEEDBACK_PITCH_LOW];
  report[rep_PITCH_HIGH] = toVerVmaPacket[TO_VER_FEEDBACK_PITCH_HIGH];
  report[rep_ROLL_LOW]   = toVerVmaPacket[TO_VER_FEEDBACK_ROLL_LOW];
  report[rep_ROLL_HIGH]  = toVerVmaPacket[TO_VER_FEEDBACK_ROLL_HIGH];
  
  report[rep_WMA_ERRORS] = fromVerVmaPacket[F_VER_FLAGS_OFFSET] || (fromHorVmaPacket[F_HOR_FLAGS_OFFSET] << 4 );

  report[rep_WATER_SENSOR] = leakage;


}
//=========================================
// преобразовать значение в градусах в целое число, 
// умещающееся в char, т.е. 0 градусов ~ 0
// 90 градусов ~ 127
// -90 градусов ~ -128
char float_to_char_90(float data)
{

  int temp= (int) constrain(data,-90.00,90.00)*100.00;

  return (map(temp,-9000,9000,-128,127));
}

// преобразовать значение в градусах в целое число, равное
// количеству сотых долей, 
// например дробное значение 169.121236 градусов превращается в целое число 16912
int float_to_int_180(float data)
{
  return((int) (data*100.0)) ;
}


void control_init()
{
  for (int i=0;i<=CONTROL_BYTES_AMOUNT;i++)
    control[i]=0;
  control[con_CAMERA_POS]=128; // среднее положение камеры
}

//число в рад/с (максимум +- 3рад/с) становится в пределах ±30 000, т.е. с точностью до десятитысячных
int angular_to_int(float data)
{
  return (int) (data*10000.0); 

}



