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
void debug_init();
void from_depth_packet_making();
void depth_sensor_feedback_sending();
void i2c_init(byte address);
void buffer_check_and_packet_write(int bytesReceived);
void main_interrupt(int howMany);

#include "C:\Arduino-1.0.5\hardware\arduino\variants\eightanaloginputs\pins_arduino.h" 
#include "C:\Arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_sensor.ino"
#include "D:\Akvator-3D-firmware\depth_sensor\debug.h"
#include "D:\Akvator-3D-firmware\depth_sensor\debug.ino"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_calculation_head.h"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_feedback.ino"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_global_variables.h"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_i2c.ino"
#include "D:\Akvator-3D-firmware\depth_sensor\depth_i2c_head.h"
#include "D:\Akvator-3D-firmware\depth_sensor\external_ADC.h"
