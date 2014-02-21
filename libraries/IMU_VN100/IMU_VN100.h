

#include <inttypes.h>

#define IMU_BUFFERSIZE 250

//#############ATTENTION!#####################
//###########LIST OF CHANGES###################
//1) Arrays are now initialized in the constructor (IMU_Class())
//2) All sign variables are now byte type (uint8_t), not float
//3) Init() function now takes a long int parameter as baudrate
//#################################################

#define X 0
#define Y 1
#define Z 2


class IMU_Class
{
	private:			// Internal Variables
	uint8_t IMU_checksum;
	uint8_t	IMU_checksum_calc;
	char buffer[IMU_BUFFERSIZE];
	int bufferidx;
	
	void parse_IMU(void);
	uint8_t parseHex(char c);
	unsigned int decimalParser(char *str, uint8_t numdec);
	
	public:				// External Variables
	// Methods
	IMU_Class();
	void Init(long baudrate);
	void Read();
	// Properties
	float Yaw;          	// IMU Yaw Reading
	//float YawUpper;
	//float YawLower;
	char YawSign;
	float Pitch; 		// IMU  Pitch Reading
	//float PitchUpper, PitchLower;
	char PitchSign;
	float Roll;			// IMU  Roll Reading
	//float RollUpper, RollLower;
	char RollSign;
	
	
	
	
	
	//char magSign[3];
	char accelSign[3];
	char angSign[3];
	
	//float Magnetic[3];
	float Acceleration[3];
	float Angular[3];
	
	
	volatile uint8_t NewData;    // 1:New IMU Data
	volatile uint8_t NewYPR;    // 1:New IMU Data
	volatile uint8_t NewAccel;    // 1:New IMU Data
	volatile uint8_t NewAng;    // 1:New IMU Data
	
	uint8_t PrintErrors; // 1: To Print IMU Errors (for debug)
};

extern IMU_Class IMU;

