void from_depth_packet_making()
{
  int intFilteredDepth = (int) filteredDepth;
  fromDepthSensorPacket[FROM_DEPTH_HIGH]=highByte(intFilteredDepth);
  fromDepthSensorPacket[FROM_DEPTH_LOW]=lowByte(intFilteredDepth);
  //depthVelocity = (byte) char((depth - oldDepth)*SYSTEM_CLOCK_FREQUENCY*100); // тут какая-то ересь, но пусть будет
  intDepthVelocity = (int) filteredVelocity;
  fromDepthSensorPacket[FROM_DEPTH_SPEED_HIGH]= highByte(intDepthVelocity);
  fromDepthSensorPacket[FROM_DEPTH_SPEED_LOW]= lowByte(intDepthVelocity);
  fromDepthSensorPacket[FROM_DEPTH_SENSOR_DATA_SIZE]=checksum_calc(fromDepthSensorPacket,FROM_DEPTH_SENSOR_DATA_SIZE);
  oldDepth = intFilteredDepth;
}

void depth_sensor_feedback_sending()
{
  //interrupts();
  Wire.write(fromDepthSensorPacket,FROM_DEPTH_SENSOR_DATA_SIZE+1);
  if (outputDebug)
  {
  Serial.print("\nfrom DEPTH: ");
  packet_printing(fromDepthSensorPacket,FROM_DEPTH_SENSOR_DATA_SIZE);
  }

}

