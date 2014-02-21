//***********МЕЖКОНТРОЛЛЕРНАЯ ШИНА I2C - РАЗМЕРЫ ПАКЕТОВ, РАЗМЕРЫ И ПОЗИЦИИ ПЕРЕМЕННЫХ ***********// 

// Частота замыкания аппарата

#define SYSTEM_CLOCK_FREQUENCY 50

//  цена деления АЦП в миллиметрах

#define DEPTH_SCALE 15 // 15 мм на 1 деление АЦП из 1024, диапазон
#define DEPTH_SCALE_ADC 3.75 //3/75 мм на деление 14-разрядного внешнего АЦП

//------------I2C OUTPUT PACKET DATA OFFSETS-------------
// То есть организация данных, передаваемых от мастера (Connection Controller) ко всем слейвам
// TO VERVMA DATA OFFSETS
enum {
 TO_VER_Z_SPEED_OFFSET, //это значит, что в пакете, отправляемом на контроллер вертикальных вма, в ячейке toVerVmaPacket[0] находится управление по вертикальной скорости

 TO_VER_FEEDBACK_PITCH_HIGH,
 TO_VER_FEEDBACK_PITCH_LOW,

 TO_VER_FEEDBACK_PITCH_SPEED_HIGH,
 TO_VER_FEEDBACK_PITCH_SPEED_LOW ,

 TO_VER_FEEDBACK_ROLL_HIGH,
 TO_VER_FEEDBACK_ROLL_LOW,

 TO_VER_FEEDBACK_ROLL_SPEED_HIGH,
 TO_VER_FEEDBACK_ROLL_SPEED_LOW,

 TO_VER_COMPLEX_DEPTH_HIGH,
 TO_VER_COMPLEX_DEPTH_LOW,
 TO_VER_COMPLEX_DEPTH_SPEED_HIGH,
 TO_VER_COMPLEX_DEPTH_SPEED_LOW,

 TO_VER_SAU_BYTE,

 TO_VER_DEPTH_SHIFT,

 TO_VERVMA_DATA_SIZE,
};

// TO HORVMA DATA OFFSETS
enum {
 TO_HOR_X_SPEED_OFFSET,
 TO_HOR_Y_SPEED_OFFSET,
 TO_HOR_YAW_SPEED_OFFSET,

 TO_HOR_FEEDBACK_YAW_HIGH,
 TO_HOR_FEEDBACK_YAW_LOW,
 TO_HOR_FEEDBACK_YAW_SPEED_HIGH,
 TO_HOR_FEEDBACK_YAW_SPEED_LOW,

 TO_HOR_SAU_BYTE,

 TO_HORVMA_DATA_SIZE,
};

// TO SERVOCONT DATA OFFSETS
enum {
 TO_SERV_LIGHT_OFFSET,
 TO_SERV_CAM_OFFSET,
 TO_SERV_GRAB_OFFSET,

 TO_SERV_SHOULDER_OFFSET,
 TO_SERV_ELBOW_OFFSET,
 TO_SERV_WRIST_OFFSET,

 TO_SERV_ROT_TOOL_LEFT,
 TO_SERV_ROT_TOOL_RIGHT,

 TO_SERVOCONT_DATA_SIZE,
};

// TO DEPTH DATA OFFSETS
enum {
	TO_DEPTH_ACCEL_OFFSET,
	TO_DEPTH_SENSOR_DATA_SIZE,
};

//-------I2C FEEDBACK DATA OFFSETS
//Расположение данных в пакетах обратной связи, идущих от слейвов на мастер

enum {
	FROM_DEPTH_HIGH,
	FROM_DEPTH_LOW,
	FROM_DEPTH_SPEED_HIGH,
	FROM_DEPTH_SPEED_LOW,

	FROM_DEPTH_SENSOR_DATA_SIZE,
};

//расположение данных с контроллера горизонталки и общий размер пакета с нее
enum {
	F_HOR_HLF_CURRENT_OFFSET,
	F_HOR_HLB_CURRENT_OFFSET,
	F_HOR_HRB_CURRENT_OFFSET,
	F_HOR_HRF_CURRENT_OFFSET,
	F_HOR_HLF_FORCE_OFFSET,
	F_HOR_HLB_FORCE_OFFSET,
	F_HOR_HRB_FORCE_OFFSET,
	F_HOR_HRF_FORCE_OFFSET,
	F_HOR_FLAGS_OFFSET,

	FROM_HORVMA_DATA_SIZE,
};

//расположение данных с контроллера вертикалки и общий размер пакета с нее
enum {
	F_VER_VLF_CURRENT_OFFSET,
	F_VER_VLB_CURRENT_OFFSET,
	F_VER_VRB_CURRENT_OFFSET,
	F_VER_VRF_CURRENT_OFFSET,
	F_VER_VLF_FORCE_OFFSET,
	F_VER_VLB_FORCE_OFFSET,
	F_VER_VRB_FORCE_OFFSET,
	F_VER_VRF_FORCE_OFFSET,
	F_VER_FLAGS_OFFSET,

	FROM_VERVMA_DATA_SIZE,
};


//===============================================
enum {
	FROM_SERVOCONT_CURRENTS_OFFSET,
	FROM_SERVOCONT_FLAGS_OFFSET,
	
	FROM_SERVOCONT_DATA_SIZE,
};

//------- VARIABLES POSITIONS IN THE DATA ARRAYS ----------
// массив moveArray[] содержит данные об управлении движением аппарата с джойстика
// В него входят 4 переменные: move_array[0] - скорость по маршу, move[1] - скорость по лагу, move[2] - скорость по углу курса, move[3] - скорость по глубине
// Аналогично строятся другие массивы данных.


enum {
	REG_X_SPEED = 0, // позиция переменной в массиве
	REG_Y_SPEED,
	REG_YAW_SPEED,
	REG_DEPTH_SPEED,

	MOVE_ARRAY_SIZE,
};
//-----------------------
//Массив camservArray[] содержит данные об управлении поворотом камер

enum {
	REG_CAM1_SERV,
	REG_CAM2_SERV,

	CAMSERV_ARRAY_SIZE,
};

//----------------------
//Массив toolsServArray[] содержит данные об управлении поворотом камер


enum {
	REG_TOOL1_SERV,
	REG_TOOL2_SERV,

	TOOLSSERV_ARRAY_SIZE,
};
//----------------------
//Массив manArray[] содержит данные об управлении манипулятором


enum {
	REG_GRAB,
	REG_JOINT1,
	REG_JOINT2,
	REG_SHOULDER,

	MAN_ARRAY_SIZE,
};
