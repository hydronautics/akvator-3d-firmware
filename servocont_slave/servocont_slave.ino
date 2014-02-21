
//*****************ПРОГРАММА КОНТРОЛЛЕРА ПРИСПОСОБЛЕНИЙ ****************//
// Изначальная среда разработки: Arduino 1.0.2
// Платформа SEEDUINO Mega
// Контроллер - Atmega1280
// Осуществляет обмен данными с контроллером связи по шине I2C.
// Осуществляет управление светом, положением камер и приспособлений.

/*
 ======== ИНСТРУКЦИЯ ПО КАЛИБРОВКЕ СЕРВОМАШИНОК МАНИПУЛЯТОРА   =====
Значения сигнала, соответствующие средним положениям сервомашинок,
представлены на вкладке tools_control_head.h в виде дефайнов FIRST_SERVO_MIDDLE, SECOND_SERVO_MIDDLE и THIRD_SERVO_MIDDLE.
Тестировать манипулятор можно двумя способами:
1. В стандартном режиме, когда манипулятор управляется с джойстика на берегу.
Для того чтобы включить отображение управляющих воздействий, подаваемых на манипулятор, 
нужно в Serial Monitore ввести команды 'c' и 'm' (controlDebug и manDebug). 
При этом будут выводиться относительные значения приращений подаваемого воздействия относительно положения SERVO_MIDDLE у каждой сервы.
Таким образом, можно плавно корректировать приращения, чтобы вычислить среднеее значение.
2. В ручном режиме рулить приращением на каждой из сервомашинок через Serial Monitor
Для этого на вкладке serv_i2c нужно закомментировать функцию tools_output и раскомментировать man_test_control();
!!! Затем прошить аппарат!!!
Далее, открыв Serial Monitor можно давать фиксированные приращения на каждую из сервомашинок 
командами 'q'/'a' (первая серва, в одну и в другую стороны), 'w'/'s', 'e'/'d'.
Величина фиксированного приращения сейчас составляет 4 единицы. 
Ниже, в коде на этой странице эту величину можно поменять.
Для вывода значений уже не нужно включать controlDebug и manDebug.
Также выводятся именно абсолютные значения приращений, отновительно SERVO_MIDDLE!!

*/

#include <Servo.h>


#include <Wire.h>            //библиотека i2c
#include "serv_i2c_head.h"  
#include "tools_control_head.h"
#include "debug.h"

#include <global_functions.h>
#include <i2c_addresses.h>
#include <i2c_packet_sizes_and_variables.h>

#include "serv_global_variables.h"
//Servo firstToolServo;
//Servo secondToolServo;
Servo cameraServo;

Servo manOneServo,manTwoServo,manThreeServo;

Servo leftToolServo,rightToolServo;

void setup()
{
  Serial.begin(DEBUG_BAUD);
  i2c_init(SERVOCONT_ADDRESS);
  tools_init();
  pwm_fix();
  Wire.onRequest(servocont_feedback_sending);
}


void loop()
{

  //Serial.println('*');

  if (Serial.available())
  {
    switch (Serial.read())
    {
    case 'o':
      outputDebug=!outputDebug; // Включаем вывод информации, отправляемой со слейва к мастеру

      break;

    case 'i':
      inputDebug=!inputDebug; // Включаем вывод информации, принимаемой слейвом от мастера
      break;

    case 'c':
      controlDebug=!controlDebug; // Включаем вывод значений света, камеры, помпы
      break;
      
      case 'q': // Меняем значения переменных, с помощью которых можно рулить манипулятором через Serial Monitor.
      if (manOneSpeed<127)
        manOneSpeed+=4;
      break;
      
      case 'a':
      if (manOneSpeed>-127)
        manOneSpeed-=4;
      break;
      
       case 'w':
      if (manTwoSpeed<127)
        manTwoSpeed+=4;
      break;
      
      case 's':
      if (manTwoSpeed>-127)
        manTwoSpeed-=4;
      break;
      
       case 'e':
      if (manThreeSpeed<127)
        manThreeSpeed+=4;
      break;
      
      case 'd':
      if (manThreeSpeed>-127)
        manThreeSpeed-=4;
      break;
      
//      case '':
//      if (manThreeSpeed>-127)
//        manThreeSpeed-=4;
//      break;
      
      
      case ' ': // Остановка всех звеньев и помпы
      manOneSpeed=0;
      manTwoSpeed=0;
      manThreeSpeed=0;
      fullStop=!fullStop;
      break;
      
      case 'm':
      manDebug=!manDebug; // Включаем вывод значений манипулятора. Должен быть включен controlDebug.
      break;
      
      case 'x':
      servSwitching=!servSwitching; // Включаем режим попеременного движения звеньев.
      break;
    }
  }

}





