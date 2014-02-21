//#define CURRENT_HALF_RANGE 127
#define CURRENT_RANGE 255

boolean answerReady=false;

void from_vervma_packet_making()
{
   fromVerVmaPacket[F_VER_VLF_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_VLF_PIN),current_VLF_null);
  fromVerVmaPacket[F_VER_VLB_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_VLB_PIN),current_VLB_null);
  fromVerVmaPacket[F_VER_VRB_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_VRB_PIN),current_VRB_null);
  fromVerVmaPacket[F_VER_VRF_CURRENT_OFFSET]=current_constrain(analogRead(SENSOR_VRF_PIN),current_VRF_null);
  
  fromVerVmaPacket[F_VER_VLF_FORCE_OFFSET]=(byte)((verVmaOutputArray[VLF]/2)*(2*verVmaDirectionArray[VLF]-1));
  fromVerVmaPacket[F_VER_VLB_FORCE_OFFSET]=(byte)-((verVmaOutputArray[VLB]/2)*(2*verVmaDirectionArray[VLB]-1));
  fromVerVmaPacket[F_VER_VRB_FORCE_OFFSET]=(byte)((verVmaOutputArray[VRB]/2)*(2*verVmaDirectionArray[VRB]-1));
  fromVerVmaPacket[F_VER_VRF_FORCE_OFFSET]=(byte)-((verVmaOutputArray[VRF]/2)*(2*verVmaDirectionArray[VRF]-1));
  fromVerVmaPacket[FROM_VERVMA_DATA_SIZE]=checksum_calc(fromVerVmaPacket,FROM_VERVMA_DATA_SIZE);
  
  answerReady=true;
}

void from_vervma_fixed_packet_making()
{
   for (int i=0;i<FROM_VERVMA_DATA_SIZE;i++)
     fromVerVmaPacket[i]=i;
  
  fromVerVmaPacket[FROM_VERVMA_DATA_SIZE]=checksum_calc(fromVerVmaPacket,FROM_VERVMA_DATA_SIZE);
}


void vervma_feedback_sending()
{
  if (!answerReady)
    Serial.println("Answer not ready!");
    
  Wire.write(fromVerVmaPacket,FROM_VERVMA_DATA_SIZE+1);
  
  if (outputDebug)
  {
    Serial.print("\nVerMsgSent: ");
    packet_printing(fromVerVmaPacket,FROM_VERVMA_DATA_SIZE);
  }

}

byte current_constrain(int current, int current_null)
{
// return (constrain(current,current_null - CURRENT_HALF_RANGE,current_null + CURRENT_HALF_RANGE) - current_null);
return constrain(abs(current-current_null),0,CURRENT_RANGE);

}
