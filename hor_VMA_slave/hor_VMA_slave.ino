//*****************ПРОГРАММА КОНТРОЛЛЕРА ГОРИЗОНТАЛЬНЫХ ВМА*****************//
// Изначальная среда разработки: Arduino 1.0.2
// Платформа - Arduino mini
// Контроллер - Atmega328P
// Осуществляет обмен данными с контроллером связи по шине I2C.
// Осуществляет управление ВМА горизонтального движения.
// ВМА №5 - HLF - горизонтальный, левый, передний
// ВМА №6 - HLB - горизонтальный, левый, задний
// ВМА №7 - HRB - горизонтальный, правый, задний
// ВМА №8 - HRF - горизонтальный, правый, передний



#include <Wire.h>            //библиотека i2c
#include "hor_i2c_head.h"  
#include "control_head.h"
#include "debug.h"


#include <global_functions.h>
#include <i2c_addresses.h>
#include <i2c_packet_sizes_and_variables.h>

//Файл hor_global_variables.h должен быть подключен после i2c_packet_sizes_and_variables.h
#include "hor_global_variables.h"

void setup()
{
  Serial.begin(DEBUG_BAUD);
  i2c_init(HORVMA_ADDRESS);
  hor_pwm_fix();
  control_init();
  
  Wire.onRequest(horvma_feedback_sending);

}

void loop()
{

  if (Serial.available())
  {
    switch (Serial.read())
    {
      
    case 's':  
      sauDebug=!sauDebug;

      break;


    case 'o':
      outputDebug=!outputDebug;

      break;

    case 'i':
      inputDebug=!inputDebug;
      break;

    case 'c':
      controlDebug=!controlDebug;
      break;
      
      case 'n':
      vnavDebug=!vnavDebug;
      break;
    }
  }
}



