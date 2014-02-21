
void debug_init() {
  Serial.begin(DEBUG_BAUD);
  Serial.println("DEPTH CONTROLLER 17.06.13\n"); 
}

//void print_sent_data() {
//   Serial.print("depth:\t");
//   Serial.print(depth/1000);
//   Serial.print("\tvel:\t");
//   Serial.println(depthVelocity);
//}
