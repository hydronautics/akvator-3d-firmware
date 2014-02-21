// Режимы работы манипулятора переключаются логической переменной servSwitching в разделе debug.
// Ее значение можно менять при компиляции, а можно
// Если true, то при движении двух звеньев, они движутся попеременно. 
//Если false, то все звенья могут двигаться одноверменно.

#define SHOULDER_ELBOW_SWITCH_FREQ  3 // Если задействован режим попеременного движения звеньев,
// то переключаются они с частотой 3 SHOULDER_ELBOW_SWITCH_FREQ Герц.

#define POWER_VOLTAGE 48

//#define ONE_JOINT_POWER 12

#define NOMINAL_PUMP_VOLTAGE 12 // Величина напряжения на помпе, когда движется только одно звено. 
// В вольтах. Было 12 В, Костанди попросил уменьшить до 8.
#define INCREASED_PUMP_VOLTAGE 12 // Напряжение на помпе, когда движутся два (или три) звена одновременно.

#define MAN1_PIN  4  // Серва манипулятора, обозначенная в эл. схеме как MAN\SERV1. Какое именно плечо - неизвестно.
#define MAN2_PIN  5 // -//-
#define MAN3_PIN  6

#define LIGHT_PIN 12
#define CAMERA_SERVO_PIN 2



#define TOOL1_SERVO_PIN 8

#define TOOL1_MIDDLE 1505

#define TOOL3_SERVO_PIN 9

#define TOOL3_MIDDLE 1495


#define GRAB_DIR_PIN A15 // Пин сжатия/расжатия схвата
#define GRAB_PWM_PIN 10 // Пин, выдающий ШИМ на схват

#define PUMP_PIN 11
#define CAM_ANGLE_UPPER  180
#define CAM_ANGLE_LOWER 10

#define HALF_HOLD_SIGNAL 64 // Сигнал с берега на полусжатие схвата

#define FULL_HOLD_SIGNAL  127 // Сигнал с берега на полное сжатие схвата

#define RELEASE_SIGNAL  (-64) // Сигнал с берега на раскрытие схвата

#define HALF_HOLD_PWM 64 // Величина ШИМ на полусжатие схвата. Например, 64 из 255 - это 12 вольт из 48.

#define FULL_HOLD_PWM  127 // Величина ШИМ на полное сжатие схвата.

#define RELEASE_PWM  64 // Величина ШИМ на расжатие схвата

#define FIRST_SERVO_MIDDLE 87 // Среднее положение сервок манипулятора.Лопатка.0 - max право,180 - max лево 

#define SECOND_SERVO_MIDDLE 96 // Среднее положение сервок манипуляторов.Плечо. 0 - max верх, 180 - max - вниз

#define THIRD_SERVO_MIDDLE 86 // Среднее положение сервок манипуляторов.Кисть. 0 - max вниз,180 max вверх

// В этом положении звенья не движутся. Смещение сервы относительно этого положения
// вызовет движение звена.

#define SERVO_LIMIT 30 /* Относительно среднего положения серва может смещаться в каждую сторону не более, чем на SERVO_LIMIT градусов. 
изначально тут было 30 градусов, затем увеличили до 60, что скорее всего неправильно */

char manOneSpeed =0; // Это отладочные переменные. С их помощью можно было рулить звеньями манипулятора не с берега, а через Serial Monitor 

char manTwoSpeed=0;

char manThreeSpeed =0;

//char leftServo

// Режим разгона и замедления помпы еще не задействован!!!

//#define PUMP_ON_DELAY 1 // Помпа включается через 1 секунду после подачи команды на движение манипулятора

#define PUMP_OFF_DELAY 60 // Помпа отключается через 60 секунд бездействия


