#define FREQUENCY 50 // текущая частота работы аппарата - 50 гц
#define SUPERDELAY floor(FREQUENCY * 0.25)


// Данных с датчика глубины пока нет :(

float Different, Kren, DifferentSpeed,KrenSpeed;
char Joy;
float k1_kren=15;
float k2_kren=1.5;
float k1_diff=15;
float k2_diff=1.5;
float k1_glub=0.12;
float k2_glub=0;
// int testValue = 10;
 int delta_time=1;
 int timeDelayGlobal;
float error_speed_kren;
float error_speed_diff;
float error_speed_glub;

float testValue_kren=0.3;
float testValue_diff=1.7;

float testIntegrator(float new_value, int index, long time_interval)
{
  static float old_value[10];
  //По методу половины прямоугольников
  static float return_value[10];
  return_value[index] = return_value[index] + (min(new_value,old_value[index]) + (max(new_value,old_value[index])-min(new_value,old_value[index]))/2)*time_interval;
  old_value[index] = new_value; 
  return return_value[index];
}

//t1 - proportional, t2 - integrate, t2_on - включен ли t2;
float testPIregulator(float new_value, int index, long time_interval, float t1, boolean t2_on, float t2)
{
 // static float old_value[10];
  //По методу половины прямоугольников
  static float return_value[10];
  return_value[index] = new_value*t1+testIntegrator(new_value,index,time_interval)*t2_on/t2;
 // old_value[index] = new_value; 
  return return_value[index];
}



int Depth;
int DepthSpeed;

void parse_feedback_data()
{
  //угол дифферента, число от -179.99 до +179.99, c точностью до сотых долей градуса
    // На практике - последний разряд в покое шумит

Different =int(word(toVerVmaPacket[TO_VER_FEEDBACK_PITCH_HIGH],toVerVmaPacket[TO_VER_FEEDBACK_PITCH_LOW]))/100.0;
  
  //угол крена, число от -179.99 до +179.99, c точностью до сотых долей градуса
Kren =int(word(toVerVmaPacket[TO_VER_FEEDBACK_ROLL_HIGH],toVerVmaPacket[TO_VER_FEEDBACK_ROLL_LOW]))/100.0;
  
  //угловая скорость вокруг оси Y, т.е. скорость дифферента, число от - 3.2768 до +3,2767 рад/с, c точностью до десятитысячных долей рад/с
    // На практике последние два разряда в покое шумят, плюс скачет знак.

DifferentSpeed =int(word(toVerVmaPacket[TO_VER_FEEDBACK_PITCH_SPEED_HIGH],toVerVmaPacket[TO_VER_FEEDBACK_PITCH_SPEED_LOW]))/10000.0;
   DifferentSpeed = DifferentSpeed*180/3.1415;

  //угловая скорость вокруг оси X, т.е. скорость крена, число от - 3.2768 до +3,2767 рад/с, c точностью до десятитысячных долей рад/с
KrenSpeed =int(word(toVerVmaPacket[TO_VER_FEEDBACK_ROLL_SPEED_HIGH],toVerVmaPacket[TO_VER_FEEDBACK_ROLL_SPEED_LOW]))/10000.0;
   KrenSpeed = KrenSpeed*180/3.1415;

}

void tau_kren()

{

 //   Joy=((char)toHorVmaPacket[TO_HOR_YAW_SPEED_OFFSET]);
   // Нету джоя на крен

  float error = testValue_kren - Kren;
  float voltage_before_pi = error * k1_kren;
  float voltage_after_pi = testPIregulator(voltage_before_pi, 1, delta_time, 1, 0,1);
  //Пока что пи выключен, для включения - изменить t2_on на 1 и t2 на значение t2;
  error_speed_kren = voltage_after_pi - KrenSpeed*k2_kren;
  if (error_speed_kren>127) error_speed_kren=127;
  if (error_speed_kren<-128) error_speed_kren=-128;
  
  if (sauDebug_kren==1)
  {
      Serial.print(testValue_kren);
    Serial.print(" kurs => "); 
    Serial.print(Kren);
 //   Serial.print(" Joy => "); 
  ///  Serial.println(Joy,DEC); 
  //  Serial.print(" Old_Joy => "); 
  //  Serial.print(OldJoy,DEC); 
        Serial.print(" signal => "); 
      Serial.println(error_speed_kren);
  }
  
  //  OldJoy = Joy;
   // if (Joy==0) timeDelay = timeDelay + 1;
  //  timeDelayGlobal = timeDelay;
}

void tau_diff()

{

 //   Joy=((char)toHorVmaPacket[TO_HOR_YAW_SPEED_OFFSET]);
   // Нету джоя на крен

  float error = testValue_diff - Different;
  float voltage_before_pi = error * k1_diff;
  float voltage_after_pi = testPIregulator(voltage_before_pi, 2, delta_time, 1, 0,1);
  //Пока что пи выключен, для включения - изменить t2_on на 1 и t2 на значение t2;
  error_speed_diff = voltage_after_pi - DifferentSpeed*k2_diff;
  if (error_speed_diff>127) error_speed_diff=127;
  if (error_speed_diff<-128) error_speed_diff=-128;
  
  if (sauDebug_diff==1)
  {
      Serial.print(testValue_diff);
    Serial.print(" kurs => "); 
    Serial.print(Different);
    Serial.print(" speed => "); 
    Serial.print(DifferentSpeed); 
  //  Serial.print(" Old_Joy => "); 
  //  Serial.print(OldJoy,DEC); 
        Serial.print(" signal => "); 
      Serial.println(error_speed_diff);
  }
  
  //  OldJoy = Joy;
   // if (Joy==0) timeDelay = timeDelay + 1;
  //  timeDelayGlobal = timeDelay;
}

//Depth - глубина
//DepthSpeed - скорость по глубине

int timeDelay;

void tau_glub()

{

    Joy=((char)toVerVmaPacket[TO_VER_Z_SPEED_OFFSET]);
    
  static float testValue;
  static char OldJoy;
  //static int timeDelay;
    
      if ((Joy==0)and(OldJoy!=0)) timeDelay = 0;
      if (timeDelay == SUPERDELAY) testValue=Depth; 
      
       /* if (testValue<-90)
         if (Depth>0) Depth=Depth-360;
        if (testValue>90)
         if (Depth<0) Depth=Depth+360;
   */
  float error = testValue - Depth;
  float voltage_before_pi = error * k1_glub;
  float voltage_after_pi = testPIregulator(voltage_before_pi, 3, delta_time, 1, 0,1);
  //Пока что пи выключен, для включения - изменить t2_on на 1 и t2 на значение t2;
  error_speed_glub = voltage_after_pi - DepthSpeed*k2_glub;
  if (error_speed_glub>127) error_speed_glub=127;
  if (error_speed_glub<-128) error_speed_glub=-128;
  
  
  
    OldJoy = Joy;
    if (Joy==0) timeDelay = timeDelay + 1;
    timeDelayGlobal = timeDelay;
}
