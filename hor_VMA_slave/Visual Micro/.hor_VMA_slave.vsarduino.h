/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Nano w/ ATmega328, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
float testIntegrator(float new_value, int index, long time_interval);
float testPIregulator(float new_value, int index, long time_interval, float t1, boolean t2_on, float t2);
void parse_feedback_data();
void tau();
void control_init();
void distribute_horisontal_output_char_X_Y_Yaw(char rawMarchInput, char rawLagInput, char rawYawInput);
void pwm_output();
boolean get_direction( int wmaValue, int wmaNumber );
void control_debug_print();
void wma_speed_correction();
int sign (int number);
void from_horvma_packet_making();
void horvma_feedback_sending();
byte current_constrain(int current, int current_null);
void i2c_init(byte address);
void buffer_check_and_packet_write(int bytesReceived);
void main_interrupt(int howMany);
void hor_pwm_fix();

#include "C:\Arduino-1.0.5\hardware\arduino\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\Arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\hor_VMA_slave.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\SAU.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\control.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\control_head.h"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\debug.h"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\flags.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\hor_feedback.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\hor_global_variables.h"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\hor_i2c.ino"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\hor_i2c_head.h"
#include "D:\Akvator-3D-firmware\hor_VMA_slave\pwm_fix.ino"
