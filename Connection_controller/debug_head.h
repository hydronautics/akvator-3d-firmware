//********ОТЛАДКА ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ (ЗАГОЛОВОК)********//

#include <Arduino.h>
//Включение режимов отладки:
//#define VNAV_DEBUG_ENABLED 1
//#define FEEDBACK_DEBUG_ENABLED 0
//#define I2C_DEBUG_ENABLE 0          // вывод сообщений кода i2c

boolean yprDebug=false;
boolean accelDebug=false;
boolean gyroDebug=false;
boolean feedbackDebug=false;
boolean outDebug=false;
boolean horDebug=false;
boolean verDebug=false;
boolean servDebug=false;
boolean depthDebug=false;
boolean errorsPrint=false;

boolean depthInputDebug = false;

#define PRINT_VNAV_ERRORS 1

#define STATISTICS_PRINT_ENABLE 0   //отчёт о качестве передачи
#define RS485_DEBUG_ENABLE 0        // вывод всей входящей информации 
#define RS485_PRINT_ON_RECEIVE 0    //вывод всей принимаемой информации
#define RS485_PRINT_CONTROL_DATA 0  //вывод управляющих переменных
#define ROV_SIMULATOR_ENABLE 0      //симулятор работы датчиков ТПА

#define DEBUG_PRINT_PERIOD 1000 //период вывода диагностических сообщений
#define DEBUG_BAUD 115200       //скорость вывода данных по Serial

unsigned long errors = 0;       //количество ошибок за DEBUG_PRINT_PERIOD
unsigned long correctData = 0;  //количество корректно принятых пакетов
unsigned long errorTimer = 0;   //количество корректно принятых пакетов

//Переменные для работы симулятора
#define SIMULATION_TIME_PERIOD 1

unsigned long simulationTimer = 0;
byte thrusterCount = 0;
byte directionUp = 1;


