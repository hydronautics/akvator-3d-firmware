
//*****************ПРОГРАММА КОНТРОЛЛЕРА ВЕРТИКАЛЬНЫХ ВМА*****************//
// Изначальная среда разработки: Arduino 1.0.2
// Платформа - Arduino mini
// Контроллер - Atmega328P
// Осуществляет обмен данными с контроллером связи по шине I2C.
// Осуществляет управление ВМА вертикального движения.
// ВМА №1 - VLF - вертикальный, левый, передний
// ВМА №2 - VLB - вертикальный, левый, задний
// ВМА №3 - VRB - вертикальный, правый, задний
// ВМА №4 - VRF - вертикальный, правый, передний

#include <Wire.h>            //библиотека i2c
#include "ver_i2c_head.h"  
#include "control_head.h"
#include "debug.h"

#include <global_functions.h>
#include <i2c_addresses.h>
#include <i2c_packet_sizes_and_variables.h>

//Файл ver_global_variables.h должен быть подключен после i2c_packet_sizes_and_variables.h
#include "ver_global_variables.h"

void setup()
{
  Serial.begin(DEBUG_BAUD);
  i2c_init(VERVMA_ADDRESS);
  ver_pwm_fix();
  control_init();

  //  analogWrite(VRB_PWM_PIN,255);
  //  analogWrite(VRF_PWM_PIN,255);
  //  analogWrite(VLB_PWM_PIN,255);
  //  analogWrite(VLF_PWM_PIN,255);

  Wire.onRequest(vervma_feedback_sending);
}

void loop()
{
  if (Serial.available())
  {
    switch (Serial.read())
    {

//    case 'g':
//      glubDebug=!glubDebug;
//
//      break;

    case 'k':
      sauDebug_kren=!sauDebug_kren;

      break;

    case 'f':
      sauDebug_diff=!sauDebug_diff;

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

    case 'd':
      depthDebug=!depthDebug;
      break;
    }
  }
}




