/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Mega (ATmega1280), Platform=avr, Package=arduino
*/

#define __AVR_ATmega1280__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//
void pwm_fix();
void from_servocont_packet_making();
void servocont_feedback_sending();
void i2c_init(byte address);
void buffer_check_and_packet_write(int bytesReceived);
void main_interrupt(int howMany);
void tools_init();
void tools_output(byte light,byte camera,char grab, char shoulder,char elbow,char wrist, char left_tool,char right_tool);
void man_test_control();

#include "C:\Arduino-1.0.5\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Akvator-3D-firmware\servocont_slave\servocont_slave.ino"
#include "D:\Akvator-3D-firmware\servocont_slave\debug.h"
#include "D:\Akvator-3D-firmware\servocont_slave\pwm_fix.ino"
#include "D:\Akvator-3D-firmware\servocont_slave\serv_feedback.ino"
#include "D:\Akvator-3D-firmware\servocont_slave\serv_global_variables.h"
#include "D:\Akvator-3D-firmware\servocont_slave\serv_i2c.ino"
#include "D:\Akvator-3D-firmware\servocont_slave\serv_i2c_head.h"
#include "D:\Akvator-3D-firmware\servocont_slave\tools_control.ino"
#include "D:\Akvator-3D-firmware\servocont_slave\tools_control_head.h"
