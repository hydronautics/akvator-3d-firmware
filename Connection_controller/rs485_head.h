//***************КАНАЛ СВЯЗИ RS485 (ЗАГОЛОВОК)***************//

#include <Arduino.h>
//#include <SoftwareSerial.h>

//вывод, подключённыq к MAX485
#define RE_DE_PIN 11  
//скорость передачи через RS-485
#define RS485_BAUD 115200 

//команда PING с ПК и стандартный ответ на неё
#define PING_CODE '#'
#define PING_REPLY "CONNECTION CONTR."

#define RS485_SERIAL Serial2
//время, затрачиваемое на передачу даннных по
#define TX_PAUSE 10
#define CONNECTION_LOST_PAUSE 500
unsigned long lastReceiveTime = 0;

//**************************************************************
//Управляющие переменные с ПК записываются в элементы 
//массива control[]. Нумеруются с 0. Общее число переменных
//присваивается константе CONTROL_BYTES_AMOUNT

#define con_REQUEST_FLAG 0  //запрос ответного сообщения
#define con_MARCH 1   //маршевая скорость
#define con_LAG 2     //лаговая составляющая скорости
#define con_DEPTH 3   //скорость по глубине
#define con_YAW 4     //угловая скорость по курсу
#define con_LIGHT 5         //яркость освещения
#define con_SAU_SWITCH 6     //включение-выключение САУ
#define con_MAN_ANGLE_1 7   //степени подвижности
#define con_MAN_ANGLE_2 8   //гидравлического манипулятора
#define con_MAN_ANGLE_3 9   //----
#define con_MAN_GRAB 10     //----
#define con_CAMERA_POS 11  //резерв для управления сервомашинками
#define con_ROTATION_TOOL_L 12  //приспособления для центровки уровня
#define con_ROTATION_TOOL_R 13  //----
#define con_DEPTH_SHIFT 14


#define con_YAW_BIT	2
#define con_ROLL_PITCH_BIT	1
#define con_DEPTH_BIT 0

//Длина управляющего массива
#define CONTROL_BYTES_AMOUNT 15
//Описание входящего сообщения
#define RS485_INCOMING_MSG_LENGTH (CONTROL_BYTES_AMOUNT + 4)
//команды управления, приходящие с ПК
char control[ CONTROL_BYTES_AMOUNT ];

//**************************************************************
//Переменные, оправляемые на ПК записываются в элементы 
//массива report[]. Нумеруются с 0. Общее число переменных
//присваивается константе REPORT_BYTES_AMOUNT

#define rep_HLF_CURRENT 0   //Ток через двигатели ВМА
#define rep_HLB_CURRENT 1   //----
#define rep_HRB_CURRENT 2   //----
#define rep_HRF_CURRENT 3   //----
#define rep_VLF_CURRENT 4   //----
#define rep_VLB_CURRENT 5   //----
#define rep_VRB_CURRENT 6   //----
#define rep_VRF_CURRENT 7   //----
#define rep_HLF_FORCE 8     //Тяга ВМА по результатам распределения
#define rep_HLB_FORCE 9     //на борту подводного аппарата
#define rep_HRB_FORCE 10    //----
#define rep_HRF_FORCE 11    //----
#define rep_VLF_FORCE 12    //----
#define rep_VLB_FORCE 13    //----
#define rep_VRB_FORCE 14    //----
#define rep_VRF_FORCE 15    //----
#define rep_WMA_ERRORS 16   //Ошибки с драйверов ВМА
#define rep_ERRORS 17       //Ошибки с драйверов обвески
#define rep_WATER_SENSOR 18 //Датчик затекания (1 = ОПАСНОСТЬ)
#define rep_DEPTH_LOW 19    //Показания датчика глубины
#define rep_DEPTH_HIGH 20   //----
#define rep_YAW_LOW 21      //Показанияя датчика углового положения
#define rep_YAW_HIGH 22     //----
#define rep_ROLL_LOW 23         //----
#define rep_ROLL_HIGH 24
#define rep_PITCH_LOW 25        //----
#define rep_PITCH_HIGH 26

//Длина массива данных с аппарата
#define REPORT_BYTES_AMOUNT 27
//Описание исходящего сообщения
#define RS485_SENDING_MSG_LENGTH (REPORT_BYTES_AMOUNT + 4)
//данные для отправки на ПК
char report[ REPORT_BYTES_AMOUNT ];

