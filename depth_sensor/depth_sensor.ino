#include <MsTimer2.h>

//************** ПРОГРАММА ДАТЧИКА ГЛУБИНЫ ************
// Изначальная среда разработки: Arduino 1.0.2
// Платформа Arduino Mini
// Контроллер - Atmega328p
// Осуществляет обмен данными с контроллером связи по шине I2C.
// Осуществляет передачу на датчик глубины данных с датчика положения, а также обратную связь от датчика на остальные слейвы



#include <Wire.h>
#include "depth_i2c_head.h"
#include "debug.h"
#include "external_ADC.h"


#include <global_functions.h>
#include <i2c_addresses.h>
#include <i2c_packet_sizes_and_variables.h>

#include "depth_global_variables.h"
#include "depth_calculation_head.h"


void setup()
{
  debug_init();
  i2c_init(DEPTH_ADDRESS);
  //pinMode(DEPTH_PIN, INPUT);
  ADC_init();
  Wire.onRequest(depth_sensor_feedback_sending);
  MsTimer2::set(depth_tau,getDepth);
  MsTimer2::start();
}




void loop()
{

  if (Serial.available())
  {
    switch (Serial.read())
    {
    case 'o':
      outputDebug=!outputDebug;

      break;

    case 'i':
      inputDebug=!inputDebug;
      break;

    case 'c':
      controlDebug=!controlDebug;
      break;
      
    case 'd':
      depthDebug=!depthDebug;
      break;
      
    case 's':
      depthSpeedDebug=!depthSpeedDebug;
      break;
      
      case 'a':
      adcDebug=!adcDebug;
      break;
    }
  }
  
}

