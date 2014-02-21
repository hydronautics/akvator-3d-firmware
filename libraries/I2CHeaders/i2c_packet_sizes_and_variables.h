//***********МЕЖКОНТРОЛЛЕРНАЯ ШИНА I2C - РАЗМЕРЫ ПАКЕТОВ, РАЗМЕРЫ И ПОЗИЦИИ ПЕРЕМЕННЫХ ***********// 

// Частота замыкания аппарата

#define SYSTEM_CLOCK_FREQUENCY 50

//  цена деления АЦП в миллиметрах

#define DEPTH_SCALE 15 // 15 мм на 1 деление АЦП из 1024, диапазон
#define DEPTH_SCALE_ADC 3.75 //3/75 мм на деление 14-разрядного внешнего АЦП

//---------- REGULATION DATA SIZES FROM THE COAST ============
// Размеры переменных, хранящих данные с берега, в байтах.
// Например, запись "#define REG_X_SPEED_SIZE 1" означает, что управление маршевой скоростью аппарата (задаваемой положением джойстика) хранится в переменной размером 1 байт и принимающей значения от 0 до 255
#define REG_X_SPEED_SIZE 1 //1 byte
#define REG_Y_SPEED_SIZE 1
#define REG_Z_SPEED_SIZE 1 

#define REG_YAW_SPEED_SIZE 1


#define REG_SERVO1_SIZE 1
#define REG_SERVO2_SIZE 1
#define REG_GRAB_SIZE 1
#define REG_JOINT1_SIZE 1
#define REG_JOINT2_SIZE 1
#define REG_SHOULDER_SIZE 1

#define REG_LIGHT_SIZE 1


#define REG_DEPTH_SAU_FLAG_SIZE 1
#define REG_YAW_SAU_FLAG_SIZE 1
#define REG_ROLL_SAU_FLAG_SIZE 1
#define REG_PITCH_SAU_FLAG_SIZE 1


#define TO_DEPTH_DATA1_SIZE 1

//-------- FEEDBACK DATA FROM SENSORS -------------
#define FEEDBACK_DEPTH_SIZE 2
#define FEEDBACK_YAW_SIZE 2
#define FEEDBACK_PITCH_SIZE 1
#define FEEDBACK_ROLL_SIZE 1

//-----------------------------------------
//----- I2C PACKET SIZES-------------------

//VERTICAL MOTION THRUSTER

//#define TO_VERVMA_DATA_SIZE (REG_Z_SPEED_SIZE+REG_DEPTH_SAU_FLAG_SIZE+REG_PITCH_SAU_FLAG_SIZE+REG_ROLL_SAU_FLAG_SIZE)

//#define FROM_VERVMA_DATA_SIZE 3

//HORIZONTAL MOTION THRUSTER

//#define TO_HORVMA_DATA_SIZE (REG_X_SPEED_SIZE+REG_Y_SPEED_SIZE+REG_YAW_SPEED_SIZE+REG_YAW_SAU_FLAG_SIZE)


//#define FROM_HORVMA_DATA_SIZE 3

//SERVOS CONTROLLER
//#define TO_SERVOCONT_DATA_SIZE (REG_SERVO1_SIZE+REG_SERVO2_SIZE+REG_LIGHT_SIZE+REG_GRAB_SIZE+REG_JOINT1_SIZE+REG_JOINT2_SIZE+REG_SHOULDER_SIZE)

#define FROM_SERVOCONT_DATA_SIZE 3

//DEPTH SENSOR
#define TO_DEPTH_SENSOR_DATA_SIZE (TO_DEPTH_DATA1_SIZE)

//#define FROM_DEPTH_SENSOR_DATA_SIZE 3

//------------I2C PACKET DATA OFFSETS-------------
// TO VERVMA DATA OFFSETS
#define TO_VER_Z_SPEED_OFFSET 0 //это значит, что в пакете, отправляемом на контроллер вертикальных вма, в ячейке toVerVmaPacket[0] находится управление по вертикальной скорости

#define TO_VER_FEEDBACK_PITCH_HIGH 1
#define TO_VER_FEEDBACK_PITCH_LOW 2

#define TO_VER_FEEDBACK_PITCH_SPEED_HIGH 3
#define TO_VER_FEEDBACK_PITCH_SPEED_LOW 4

#define TO_VER_FEEDBACK_ROLL_HIGH 5
#define TO_VER_FEEDBACK_ROLL_LOW 6

#define TO_VER_FEEDBACK_ROLL_SPEED_HIGH 7
#define TO_VER_FEEDBACK_ROLL_SPEED_LOW 8

#define TO_VER_COMPLEX_DEPTH_HIGH 9
#define TO_VER_COMPLEX_DEPTH_LOW 10
#define TO_VER_COMPLEX_DEPTH_SPEED_HIGH 11
#define TO_VER_COMPLEX_DEPTH_SPEED_LOW 12

#define TO_VER_SAU_BYTE 13

#define TO_VER_DEPTH_SHIFT 14

#define TO_VERVMA_DATA_SIZE 15

// TO HORVMA DATA OFFSETS
#define TO_HOR_X_SPEED_OFFSET 0
#define TO_HOR_Y_SPEED_OFFSET 1
#define TO_HOR_YAW_SPEED_OFFSET 2

#define TO_HOR_FEEDBACK_YAW_HIGH 3
#define TO_HOR_FEEDBACK_YAW_LOW 4
#define TO_HOR_FEEDBACK_YAW_SPEED_HIGH 5
#define TO_HOR_FEEDBACK_YAW_SPEED_LOW 6

#define TO_HOR_SAU_BYTE	7

#define TO_HORVMA_DATA_SIZE 8

// TO SERVOCONT DATA OFFSETS
#define TO_SERV_LIGHT_OFFSET 0
#define TO_SERV_CAM_OFFSET 1
#define TO_SERV_GRAB_OFFSET 2

#define TO_SERV_SHOULDER_OFFSET 3
#define TO_SERV_ELBOW_OFFSET 4
#define TO_SERV_WRIST_OFFSET 5

//#define TO_SERV_PUMP_OFFSET 6

 #define TO_SERV_ROT_TOOL_LEFT 6
 #define TO_SERV_ROT_TOOL_RIGHT 7

#define TO_SERVOCONT_DATA_SIZE 8

// TO DEPTH DATA OFFSETS
#define TO_DEPTH_ACCEL_OFFSET 0

//-------I2C FEEDBACK DATA OFFSETS
//Расположение данных в пакетах обратной связи, идущих от слейвов на мастер

#define FROM_DEPTH_HIGH 0
#define FROM_DEPTH_LOW 1
#define FROM_DEPTH_SPEED_HIGH 2
#define FROM_DEPTH_SPEED_LOW 3

#define FROM_DEPTH_SENSOR_DATA_SIZE 4

//расположение данных с контроллера горизонталки и общий размер пакета с нее
#define F_HOR_HLF_CURRENT_OFFSET 0
#define F_HOR_HLB_CURRENT_OFFSET 1
#define F_HOR_HRB_CURRENT_OFFSET 2
#define F_HOR_HRF_CURRENT_OFFSET 3
#define F_HOR_HLF_FORCE_OFFSET 4
#define F_HOR_HLB_FORCE_OFFSET 5
#define F_HOR_HRB_FORCE_OFFSET 6
#define F_HOR_HRF_FORCE_OFFSET 7
#define F_HOR_FLAGS_OFFSET 8

#define FROM_HORVMA_DATA_SIZE 9

//расположение данных с контроллера вертикалки и общий размер пакета с нее
#define F_VER_VLF_CURRENT_OFFSET 0
#define F_VER_VLB_CURRENT_OFFSET 1
#define F_VER_VRB_CURRENT_OFFSET 2
#define F_VER_VRF_CURRENT_OFFSET 3
#define F_VER_VLF_FORCE_OFFSET 4
#define F_VER_VLB_FORCE_OFFSET 5
#define F_VER_VRB_FORCE_OFFSET 6
#define F_VER_VRF_FORCE_OFFSET 7
#define F_VER_FLAGS_OFFSET 8

#define FROM_VERVMA_DATA_SIZE 9


//===============================================
#define FROM_SERVOCONT_CURRENTS_OFFSET 0
#define FROM_SERVOCONT_FLAGS_OFFSET 1

//------- VARIABLES POSITIONS IN THE DATA ARRAYS ----------
// массив moveArray[] содержит данные об управлении движением аппарата с джойстика
// В него входят 4 переменные: move_array[0] - скорость по маршу, move[1] - скорость по лагу, move[2] - скорость по углу курса, move[3] - скорость по глубине
// Аналогично строятся другие массивы данных.
#define MOVE_ARRAY_SIZE 4

#define REG_X_SPEED 0 // позиция переменной в массиве
#define REG_Y_SPEED 1
#define REG_YAW_SPEED 2
#define REG_DEPTH_SPEED 3
//-----------------------
//Массив camservArray[] содержит данные об управлении поворотом камер
#define CAMSERV_ARRAY_SIZE 2

#define REG_CAM1_SERV 0
#define REG_CAM2_SERV 1
//----------------------
//Массив toolsServArray[] содержит данные об управлении поворотом камер
#define TOOLSSERV_ARRAY_SIZE 2

#define REG_TOOL1_SERV 0
#define REG_TOOL2_SERV 1
//----------------------
//Массив manArray[] содержит данные об управлении манипулятором
#define MAN_ARRAY_SIZE 4

#define REG_GRAB 0
#define REG_JOINT1 1
#define REG_JOINT2 2
#define REG_SHOULDER 3
