/*
*****************ПРОГРАММА КОНТРОЛЛЕРА СВЯЗИ*****************
Изначальная среда разработки: Arduino 1.0.1
Первоначальная проверка работоспособности кода в среде Arduino 1.0.2 ошибок не выявила.
Февраль 2013 - разработка в Arduino 1.0.5
Платформа - Seeeduino MEGA
Контроллер Atmega1280
Осуществляет приём, передачу и расшифровку сообщений от ПК по каналу 
связи rs485. Рассылает информацию остальным контроллерам по
шине i2c. Принимает показания датчиков и отправляет их на ПК.
*/
#include <IMU_VN100.h>
#include <Wire.h>            //библиотека i2c
#include "rs485_head.h"
#include "i2c_head.h"  //содержит вспомогательные заголовки для работы шины I2C на контроллере связи
#include "debug_head.h" //режимы отладки

/*
------------ВНИМАНИЕ---------------
Следующие header-файлы не лежат в папке с этим же скетчем
Они лежат в папке "libraries/I2CHeaders" 
libraries - это специальная папка, где можно хранить пользовательские библиотеки, при условии что ее родительская папка установлена как Sketchbook
location по умолчанию.
Эти header файлы можно подключить из любого скетча, лежащего рядом с папкой Libraries, при этом их адреса заключаются в угловые скобки вместо кавычек
*/

#include <global_functions.h> // универсальные функции для работы с пакетами, используемые в разных контроллерах на борту аппарата
#include <i2c_addresses.h> //адреса всех устройств на шине I2C
#include <i2c_packet_sizes_and_variables.h> //размеры пакетов, размеры переменных (в байтах) и их позиции в пакетах и в массивах данных

//-----------ВНИМАНИЕ---------------
// Размерности глобальных переменных определяются через дефайны, объявленные в предыдущем h-файле, поэтому их надо объявлять после i2c_packet_sizes_and_variables.h
#include "global_variables.h" //глобальные переменные для функций бортового канала связи


void setup() 
{
  debug_init();  //монитор отладки Serial
  rs485_init();  //канал связи RS485
  i2c_init(MASTER_ADDRESS); //инициализация i2c, присоединение к шине в качестве мастера
  vectornav_init();
  control_init();
  
  start_system_clock();     //запустить генерацию синхроимпульсов на пине D2
  
  pinMode(DEBUG_PIN,OUTPUT);
  digitalWrite(DEBUG_PIN,LOW);
}

void loop() 
{
  if (Serial.available())
  {
    switch (Serial.read())
    {
    case 'o':
      outDebug=!outDebug;
      if (outDebug)
        Serial.println("\nOutput Debug is ON");
        else
        Serial.println("\nOutput Debug is OFF");
      break;
      
    case 'y':
      yprDebug=!yprDebug;
      break;
      
      case 'a':
      accelDebug=!accelDebug;
      break;
      
      case 'g':
     gyroDebug=!gyroDebug;
      break;
      
    case 'f':
      feedbackDebug=!feedbackDebug;
      if (feedbackDebug)
        Serial.println("\nFeedback Debug is ON");
        else
        Serial.println("\nFeedback Debug is OFF");
      break;
      
      case 'h':
      horDebug=!horDebug;
      break;
      
      case 'v':
      verDebug=!verDebug;
      break;
      
      case 's':
      servDebug=!servDebug;
      break;
      
      case 'd':
      depthDebug=!depthDebug;
      break;
      
      case 'e':
      errorsPrint=!errorsPrint;
      if (errorsPrint)
        Serial.println("\nErrors Print is ON");
        else
        Serial.println("\nErrors Print is OFF");
      break;
      
      case 'n':
      IMU.PrintErrors=!IMU.PrintErrors;
      if (IMU.PrintErrors)
        Serial.println("\nVECTORNAV ERRORS Print is ON");
        else
        Serial.println("\nVECTORNAV ERRORS Print is OFF");
      break;
      
      case 'x':
      depthInputDebug = !depthInputDebug;
      break;
      
      
//      case 't':
//      
//      break;
    }

  }
  //если установлен флаг отладки RS485
  if ( RS485_DEBUG_ENABLE == 1 ) {
    //вызываем подпрограмму отладки
    rs485_debug();
  }
  //если во входном буфере есть хотя бы одна посылка
  if ( RS485_SERIAL.available() >= RS485_INCOMING_MSG_LENGTH ) {
    //принимаем и расшифровываем посылку
    rs485_receive();
    //если посылка требует ответа
    if ( control[ con_REQUEST_FLAG ] == 1 ) {
      //формируем и отправляем ответное сообщение
      rs485_reply();
    }
  } else if (RS485_SERIAL.available() == 1 && RS485_SERIAL.peek() == int(PING_CODE)) {
      RS485_SERIAL.read();
      rs485_driver_enable();
      rs485_send_to_uart(PING_REPLY);
      rs485_receiver_enable(); 
      Serial.println("PING RECEIVED & REPLIED");
  }
  //если установлен флаг вывода статистических данных
  if ( STATISTICS_PRINT_ENABLE == 1 ) {
    //выводим статистику на монитор отладки
    print_statistics();
  }
  //пока у нас нет реальных данных, используем 
  //симулятор работы подводного аппарата
  if (ROV_SIMULATOR_ENABLE == 1) { 
    rs485_rov_simulator();
  }
  //проверяем наличие связи
  if (millis() - lastReceiveTime > CONNECTION_LOST_PAUSE) {
    autopilot_mode_on();
    lastReceiveTime = millis();
  }
}

void autopilot_mode_on() {
  RS485_SERIAL.flush(); 
}
// В файле I2c.ino описан алгоритм обработки главного прерывания, т.е. рассылка пакетов, а также описаны вспомогательные функции

