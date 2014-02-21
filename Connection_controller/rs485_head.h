//***************КАНАЛ СВЯЗИ RS485 (ЗАГОЛОВОК)***************//

#include <Arduino.h>


//вывод, подключённыq к MAX485
// RE - Receiver Enable, DE - Driver Enable
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

enum {
	con_REQUEST_FLAG,  //запрос ответного сообщения
	con_MARCH,   //маршевая скорость
	con_LAG,     //лаговая составляющая скорости
	con_DEPTH,   //скорость по глубине
	con_YAW,     //угловая скорость по курсу
	con_LIGHT,         //яркость освещения
	con_SAU_SWITCH,     //включение-выключение САУ
	con_MAN_ANGLE_1,   //степени подвижности
	con_MAN_ANGLE_2,   //гидравлического манипулятора
	con_MAN_ANGLE_3,   //----
	con_MAN_GRAB,     // схват манипулятора
	con_CAMERA_POS,  //резерв для управления сервомашинками
	con_ROTATION_TOOL_L,  //приспособления для центровки уровня
	con_ROTATION_TOOL_R,  //----
	con_DEPTH_SHIFT,	// фиксированное смещение для стабилизации по глубине (?)
	//Длина управляющего массива
	CONTROL_BYTES_AMOUNT,
};

// номера бит в байте con_SAU_SWITCH, т.е. флагов включения/выключени контуров системы стабилизации
#define con_YAW_BIT	2	// стабилизация по курсу
#define con_ROLL_PITCH_BIT	1	// стабилизация по дифференту
#define con_DEPTH_BIT 0	// стабилизация по глубине


//Описание входящего сообщения
#define RS485_INCOMING_MSG_LENGTH (CONTROL_BYTES_AMOUNT + 4)
//команды управления, приходящие с ПК
char control[ CONTROL_BYTES_AMOUNT ];

//**************************************************************
//Переменные, оправляемые на ПК записываются в элементы 
//массива report[]. Нумеруются с 0. Общее число переменных
//присваивается константе REPORT_BYTES_AMOUNT

enum {
	rep_HLF_CURRENT,   //Ток через двигатели ВМА
	rep_HLB_CURRENT,   //----
	rep_HRB_CURRENT,   //----
	rep_HRF_CURRENT,   //----
	rep_VLF_CURRENT,   //----
	rep_VLB_CURRENT,   //----
	rep_VRB_CURRENT,   //----
	rep_VRF_CURRENT,   //----
	rep_HLF_FORCE,     //Тяга ВМА по результатам распределения
	rep_HLB_FORCE,     //на борту подводного аппарата
	rep_HRB_FORCE,    //----
	rep_HRF_FORCE,    //----
	rep_VLF_FORCE,    //----
	rep_VLB_FORCE,    //----
	rep_VRB_FORCE,    //----
	rep_VRF_FORCE,    //----
	rep_WMA_ERRORS,   //Ошибки с драйверов ВМА
	rep_ERRORS,       //Ошибки с драйверов обвески
	rep_WATER_SENSOR, //Датчик затекания (1 = ОПАСНОСТЬ)
	rep_DEPTH_LOW,    //Показания датчика глубины
	rep_DEPTH_HIGH,   //----
	rep_YAW_LOW,      //Показанияя датчика углового положения
	rep_YAW_HIGH,     //----
	rep_ROLL_LOW,         //----
	rep_ROLL_HIGH,
	rep_PITCH_LOW,        //----
	rep_PITCH_HIGH,

	//Длина массива данных с аппарата
	REPORT_BYTES_AMOUNT,
};

//Описание исходящего сообщения
#define RS485_SENDING_MSG_LENGTH (REPORT_BYTES_AMOUNT + 4)
//данные для отправки на ПК
char report[ REPORT_BYTES_AMOUNT ];

