void from_servocont_packet_making()
{
  fromServocontPacket[FROM_SERVOCONT_CURRENTS_OFFSET]=55;
  fromServocontPacket[FROM_SERVOCONT_FLAGS_OFFSET]=44;
  fromServocontPacket[FROM_SERVOCONT_DATA_SIZE]=  checksum_calc(fromServocontPacket,FROM_SERVOCONT_DATA_SIZE);
  
}

void servocont_feedback_sending()
{
  Wire.write(fromServocontPacket,FROM_SERVOCONT_DATA_SIZE+1);
  if (outputDebug)
  {
    Serial.print("\nServMsgSent: ");
    packet_printing(fromServocontPacket,FROM_SERVOCONT_DATA_SIZE);
  }

}

