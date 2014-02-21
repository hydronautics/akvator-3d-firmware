//************** ОТЛАДКА ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ **************//

//ИНИЦИАЛИЗАЦИЯ МОНИТОРА ОТЛАДКИ
void debug_init() 
{
  Serial.begin(DEBUG_BAUD);
  Serial.println("\nCONNECTION CONTROLLER PROG, Hardware Serial");
}  


//ПОДПРОГРАММА ВЫВОДА СТАТИСТИЧЕСКОЙ ИНФОРМАЦИИ
void print_statistics()
{
  if ( millis() - errorTimer >= DEBUG_PRINT_PERIOD ) {
    errorTimer = millis();
    float freq = ( float( errors + correctData ) / DEBUG_PRINT_PERIOD ) * 1000;
    Serial.print( "Err: ");
    Serial.print( errors, DEC );
    Serial.print(",  Corr: " );
    Serial.print( correctData, DEC );
    Serial.print(",  Freq: " );
    Serial.println( freq, DEC );
   
    errors = 0;
    correctData = 0;
  }
}


//ПОДПРОГРАММА ВЫВОДА ВХОДЯЩЕЙ ИНФОРМАЦИИ ПО RS485
void rs485_debug() 
{
  long timer = 0;
  while ( true ) {
    if ( RS485_SERIAL.available() ) {
      while ( RS485_SERIAL.available() ) {
        Serial.print( RS485_SERIAL.read(), DEC );
        Serial.print(' ');
      }
      Serial.println();
      Serial.println( millis() - timer, DEC );
      timer=millis();
      rs485_driver_enable();
      rs485_receiver_enable();
    }
  }
}


//ПОДПРОГРАММА ВЫВОДА УПРАВЛЯЮЩИХ КОМАНД В SERIAL MONITOR
void rs485_print_control()
{
  Serial.print("yaw: ");
  Serial.print(control[con_YAW], DEC);
  Serial.print(";  march: ");
  Serial.print(control[con_MARCH], DEC);
  Serial.print(";  depth: ");
  Serial.print(control[con_DEPTH], DEC);
  Serial.print(";  lag: ");
  Serial.print(control[con_LAG], DEC);
  Serial.print(";  light: ");
  Serial.print(control[con_LIGHT], DEC);
  Serial.print(";  sau: ");
  Serial.print(control[con_SAU_SWITCH], BIN);
  Serial.print(";  cam: ");
  Serial.print(control[con_CAMERA_POS], DEC);
  Serial.print(";  L tool: ");
  Serial.print(control[con_ROTATION_TOOL_L], DEC);
  Serial.print(";  R tool: ");
  Serial.println(control[con_ROTATION_TOOL_R], DEC);
}


//ПОДПРОГРАММА, СИМУЛИРУЮЩАЯ РАБОТУ ТПА
//СЛУЖИТ ДЛЯ ЗАПОЛНЕНИЯ ПАКЕТA REPLY[]
void rs485_rov_simulator() {
  if (millis() - simulationTimer > SIMULATION_TIME_PERIOD) {
    simulationTimer = millis();
    int force = report[rep_HLF_FORCE + thrusterCount];
    if (directionUp == 1) {
      force += 1;
      if (force > 127) {
        force = 127;
        directionUp = 0;
      }
    } else {
      force -= 1;
      if (force < -127) {
        force = -127;
        directionUp = 1;
      }
    }
    report[rep_HLF_FORCE + thrusterCount] = force;
    if (force == 0 && directionUp == 1) {
      thrusterCount += 1;
      if (thrusterCount > 7) {
        thrusterCount = 0; 
      }
    } 
  }
}
