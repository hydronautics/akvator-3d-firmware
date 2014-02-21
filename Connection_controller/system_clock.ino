/*
***************** ПОДПРОГРАММА ГЕНЕРАТОРА СИНХРОИМПУЛЬСОВ ***************** //
Среда разработки: Arduino 1.0.2
Функция start_system_clock принимает один параметр freq - частота следования импульсов.
Форма импульсов - меандр, т.е. коэффициент заполнения 0.5
Функция start_system_clock вызывается один раз, при инициализации контроллера связи.

SYSTEM_CLOCK_FREQUENCY определяется в i2c_packet_sizes_and_variables.h
f*/


void start_system_clock()
{ 
  int code=1000000/SYSTEM_CLOCK_FREQUENCY+1;
  
  TCCR3B=0b00001010;
  TCCR3A=0b00010000;
  DDRE=0b11111111;
  OCR3A=code ;
}
