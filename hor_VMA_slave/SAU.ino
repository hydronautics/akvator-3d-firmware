#define FREQUENCY 50 // текущая частота работы аппарата - 50 гц
#define SUPERDELAY floor(FREQUENCY * 0.25)

float Kurs,KursSpeed;
char Joy;
float k1=30;
float k2=3;
// int testValue = 10;
 int delta_time=1;
 int timeDelayGlobal;
float error_speed;

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

void parse_feedback_data()  // эта функция записывает значения в переменные Kurs и KursSpeed
{
  //угол курса, число от -179.99 до +179.99, c точностью до сотых долей градуса
  // На практике - последний разряд в покое шумит
Kurs =int(word(toHorVmaPacket[TO_HOR_FEEDBACK_YAW_HIGH],toHorVmaPacket[TO_HOR_FEEDBACK_YAW_LOW]))/100.0;
  

  
  //угловая скорость вокруг оси Z т.е. скорость курса, число от - 3.2768 до +3,2767 рад/с, c точностью до десятитысячных долей рад/с
  // На практике последние два разряда в покое шумят, плюс скачет знак.
KursSpeed =int(word(toHorVmaPacket[TO_HOR_FEEDBACK_YAW_SPEED_HIGH],toHorVmaPacket[TO_HOR_FEEDBACK_YAW_SPEED_LOW]))/10000.0;
  
 KursSpeed = KursSpeed*180/3.1415;
}

//float testValue;
//char OldJoy;

int timeDelay;

void tau()

{

    Joy=((char)toHorVmaPacket[TO_HOR_YAW_SPEED_OFFSET]);
    
  static float testValue;
  static char OldJoy;
  //static int timeDelay;
    
      if ((Joy==0)and(OldJoy!=0)) timeDelay = 0;
      if (timeDelay == SUPERDELAY) testValue=Kurs; 
      
        if (testValue<-90)
         if (Kurs>0) Kurs=Kurs-360;
        if (testValue>90)
         if (Kurs<0) Kurs=Kurs+360;
   
  float error = testValue - Kurs;
  float voltage_before_pi = error * k1;
  float voltage_after_pi = testPIregulator(voltage_before_pi, 1, delta_time, 1, 0,1);
  //Пока что пи выключен, для включения - изменить t2_on на 1 и t2 на значение t2;
  error_speed = voltage_after_pi - KursSpeed*k2;
  if (error_speed>127) error_speed=127;
  if (error_speed<-128) error_speed=-128;
  
  if (sauDebug==1)
  {
      Serial.print(testValue);
    Serial.print(" kurs => "); 
    Serial.print(Kurs);
    Serial.print(" Joy => "); 
    Serial.println(Joy,DEC); 
  //  Serial.print(" Old_Joy => "); 
  //  Serial.print(OldJoy,DEC); 
  //      Serial.print(" signal => "); 
   //    Serial.println(error_speed);
  }
  
    OldJoy = Joy;
    if (Joy==0) timeDelay = timeDelay + 1;
    timeDelayGlobal = timeDelay;
}
