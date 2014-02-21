//#define CURRENT_HALF_RANGE 127
#define CURRENT_RANGE 255

void from_horvma_packet_making()
{
  fromHorVmaPacket[F_HOR_HLF_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_HLF_PIN),current_HLF_null);
  fromHorVmaPacket[F_HOR_HLB_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_HLB_PIN),current_HLB_null);
  fromHorVmaPacket[F_HOR_HRB_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_HRB_PIN),current_HRB_null);
  fromHorVmaPacket[F_HOR_HRF_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_HRF_PIN),current_HRF_null);
  
  fromHorVmaPacket[F_HOR_HLF_FORCE_OFFSET]=(byte)-((horVmaOutputArray[HLF]/2)*(2*horVmaDirectionArray[HLF]-1));
  fromHorVmaPacket[F_HOR_HLB_FORCE_OFFSET]=(byte)-((horVmaOutputArray[HLB]/2)*(2*horVmaDirectionArray[HLB]-1));
  fromHorVmaPacket[F_HOR_HRB_FORCE_OFFSET]=(byte)((horVmaOutputArray[HRB]/2)*(2*horVmaDirectionArray[HRB]-1));
  fromHorVmaPacket[F_HOR_HRF_FORCE_OFFSET]=(byte)((horVmaOutputArray[HRF]/2)*(2*horVmaDirectionArray[HRF]-1));
  
    fromHorVmaPacket[F_HOR_FLAGS_OFFSET]=digitalRead(FLAG_HLF_PIN)<<FLAG_HLF_BIT || digitalRead(FLAG_HLB_PIN)<<FLAG_HLB_BIT || digitalRead(FLAG_HRB_PIN)<<FLAG_HRB_BIT || digitalRead(FLAG_HRF_PIN)<<FLAG_HRF_BIT;

  
  fromHorVmaPacket[FROM_HORVMA_DATA_SIZE]=checksum_calc(fromHorVmaPacket,FROM_HORVMA_DATA_SIZE);
  
}

void horvma_feedback_sending()
{
  Wire.write(fromHorVmaPacket,FROM_HORVMA_DATA_SIZE+1);
  if (outputDebug)
  {
  Serial.print("\nHorMsgSent: ");
  packet_printing(fromHorVmaPacket,FROM_HORVMA_DATA_SIZE);
  }
}

byte current_constrain(int current, int current_null)
{
// return (constrain(current,current_null - CURRENT_HALF_RANGE,current_null + CURRENT_HALF_RANGE) - current_null);
 return constrain(abs(current-current_null),0,CURRENT_RANGE);

}
