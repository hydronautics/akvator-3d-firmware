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
void autopilot_mode_on();
void parse_depth();
void complex();
void to_ver_vma_packet_making();
void to_hor_vma_packet_making();
void to_servocont_packet_making();
void to_depth_packet_making();
void packet_request_and_reading(byte packet[],int address,int howManyBytes,boolean debug);
void reset_a_slave(byte reset_pin);
void report_making();
char float_to_char_90(float data);
int float_to_int_180(float data);
void control_init();
int angular_to_int(float data);
void debug_init();
void print_statistics();
void rs485_debug();
void rs485_print_control();
void rs485_rov_simulator();
void vectornav_init();
void i2c_init(byte address);
void main_interrupt();
void rs485_init();
void rs485_receive ();
int rs485_get_number_of_packets();
boolean rs485_zero_decoding(byte msg[]);
boolean rs485_check_csum(byte msg[]);
void rs485_update_controls(byte msg[]);
void rs485_reply();
void rs485_receiver_enable();
void rs485_driver_enable();
void rs485_form_message (byte msg[]);
void rs485_send_to_uart(byte msg[]);
void rs485_send_to_uart(char msg[]);
void start_system_clock();
void read_register(byte number);
void addEnding(char msg[]);

#include "C:\Arduino-1.0.5\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Arduino-1.0.5\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Akvator-3D-firmware\Connection_controller\Connection_controller.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\complex.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\conContr_functions.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\debug.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\debug_head.h"
#include "D:\Akvator-3D-firmware\Connection_controller\global_variables.h"
#include "D:\Akvator-3D-firmware\Connection_controller\i2c.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\i2c_head.h"
#include "D:\Akvator-3D-firmware\Connection_controller\rs485.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\rs485_head.h"
#include "D:\Akvator-3D-firmware\Connection_controller\system_clock.ino"
#include "D:\Akvator-3D-firmware\Connection_controller\vnav_communication.ino"
