//*********** Номера пинов и вспомогательные константы для контроллера связи ***********//

#include <Arduino.h>

#define INT_NUMBER 5 //номер внешнего прерывания, пин D18 на Seeeduino Mega

#define FIXED_PACKETS_AMOUNT 255	//количество фиксированных пакетов в генерируемом массиве 

#define VNAV_BAUDRATE 128000

#define HOR_VER_RESET_PIN A7

#define DEBUG_PIN 3

#define LEAK_PIN A0

#define LEAK_THRESHOLD 745

