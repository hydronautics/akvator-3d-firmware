//********ОТЛАДКА ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ (ЗАГОЛОВОК)********//

#include <Arduino.h>

// флаги включения/выключения вывода различных отладочных сообщений
boolean yprDebug=false;	// Yaw, Pitch, Roll - данные об углах с датчика положения
boolean accelDebug=false;	// ускорение с датчика положения
boolean gyroDebug=false;	// угловые скорости
boolean feedbackDebug=false;	// данные от слейвов  к мастеру
boolean outDebug=false;	// данные от мастера к слейвам, общий флаг для всех
boolean horDebug=false;	// вкл/выкл данных от контроллера горизонталок
boolean verDebug=false;	// от контроллера вертикалок
boolean servDebug=false; // от контроллера приспособлений
boolean depthOutDebug=false;	// данные от мастера до датчика глубины
boolean vectorNavErrors=false;	// вывод ошибок датчика положения

boolean depthInputDebug = false;	// данные с датчика глубины


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


