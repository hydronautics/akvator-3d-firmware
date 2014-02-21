byte directionPin[4];
byte pwmPin[4];

int horVmaOutputArray[4]; //значения для подачи на ВМА, т.е. от 0 до 255
//Внимание: этот же массив используется для хранения промежуточных значений при расчете 
boolean horVmaDirectionArray[4]; //значения направления вращения ВМА

int current_HLB_null;
int current_HLF_null;
int current_HRF_null;
int current_HRB_null;

void control_init()
{

  directionPin[HLF]=HLF_DIR_PIN;
  directionPin[HLB]=HLB_DIR_PIN;
  directionPin[HRB]=HRB_DIR_PIN;
  directionPin[HRF]=HRF_DIR_PIN;

  pwmPin[HLF]=HLF_PWM_PIN;
  pwmPin[HLB]=HLB_PWM_PIN;
  pwmPin[HRB]=HRB_PWM_PIN;
  pwmPin[HRF]=HRF_PWM_PIN;


  for (int i = 0; i<=3;i++)
  {
    pinMode(directionPin[i],OUTPUT);
    pinMode(pwmPin[i],OUTPUT);
    digitalWrite(pwmPin[i],LOW);
    horVmaOutputArray[i]=0;
    horVmaDirectionArray[i]=true;
  }

  pwm_output();

  current_HLB_null=analogRead(SENSOR_HLB_PIN);
  current_HLF_null=analogRead(SENSOR_HLF_PIN);
  current_HRF_null=analogRead(SENSOR_HRF_PIN);
  current_HRB_null=analogRead(SENSOR_HRB_PIN);
  
  Serial.println("Current sensors nulls are:");
  Serial.print("HLF "); Serial.println(current_HLF_null);
  Serial.print("HLB "); Serial.println(current_HLB_null);
  Serial.print("HRB "); Serial.println(current_HRB_null);
  Serial.print("HRF "); Serial.println(current_HRF_null);
}

void distribute_horisontal_output_char_X_Y_Yaw(char rawMarchInput, char rawLagInput, char rawYawInput)
{
  //переменные типа char хранят целые числа от -128 до 127. Raw - значит сырые, неприведённые.
  //по маршу +127 - полный вперед, -128 - полный назад
  // по лагу +127 - полный лаг вправо, -128 - полный лаг влево
  // по курсу +127 - максимальная скорость поворота направо, -128 - максимальная скорость поворота налево

  //суммируем значения для подачи на каждый из ВМА в зависимости от значений курса, лага и марша
  // В общем случае число в каждой ячейке может быть больше 255, т.е. надо будет затем промасштабировать

//  horVmaOutputArray[HLF]=error_speed;
// horVmaOutputArray[HLB]=-error_speed;
// horVmaOutputArray[HRB]=error_speed;
//  horVmaOutputArray[HRF]=-error_speed;


  if((rawYawInput==0) and (timeDelayGlobal>SUPERDELAY))
{
  horVmaOutputArray[HLF]=rawMarchInput+rawLagInput+error_speed;
  horVmaOutputArray[HLB]=-rawMarchInput+rawLagInput-error_speed;
  horVmaOutputArray[HRB]=-rawMarchInput-rawLagInput+error_speed;
  horVmaOutputArray[HRF]=rawMarchInput-rawLagInput-error_speed;
}


else
{
  horVmaOutputArray[HLF]=rawMarchInput+rawLagInput+rawYawInput;
  horVmaOutputArray[HLB]=-rawMarchInput+rawLagInput-rawYawInput;
  horVmaOutputArray[HRB]=-rawMarchInput-rawLagInput+rawYawInput;
  horVmaOutputArray[HRF]=rawMarchInput-rawLagInput-rawYawInput;
}
 

  // находим максимальное значение для управления ВМА, по которому будем масштабировать
  int maxValue=0;
  for (int i = 0;i<=3;i++)
    maxValue=max(maxValue,abs(horVmaOutputArray[i]));


  for (int i = 0; i<= 3; i++)
  {
    horVmaDirectionArray[i] = get_direction(horVmaOutputArray[i],i);
    if (maxValue > 127)
      horVmaOutputArray[i] = map (abs(horVmaOutputArray[i]),0,maxValue,0,255);
    else
      horVmaOutputArray[i] = map (abs(horVmaOutputArray[i]),0,127,0,255);
  }

  wma_speed_correction(); //ограничение скорости нарастания сигналов на ВМА 

  //вывод переменных в Serial монитор
  if (controlDebug) {
    control_debug_print();
  }

}

void pwm_output()
{
  //вывод ШИМ-сигналов на ВМА
  for (int i = 0; i <= 3; i++) 
  {
    analogWrite(pwmPin[i], horVmaOutputArray[i]);
    digitalWrite(directionPin[i], horVmaDirectionArray[i]);
  }

}

boolean get_direction( int wmaValue, int wmaNumber ) 
{
  boolean forseDirection = (wmaValue >= 0);  //направление тяги ВМА, больше нуля true, меньше нуля false
  boolean leftPropeller = (wmaNumber<2);
  return forseDirection ^ leftPropeller;    //инверсия тяги ВМА 1 и 2
}


void control_debug_print()
{
  Serial.println();
  for (int i=0; i<=3; i++)
  {
    Serial.print('\t');
    Serial.print("N: ");
    Serial.print(i);
    Serial.print(" DIR: ");
    Serial.print(horVmaDirectionArray[i]);
    Serial.print(" OUT: ");
    Serial.print(horVmaOutputArray[i]);
  }
}


void wma_speed_correction()
{
  static int oldSignedVmaArray[4]; //знаковые величины сигналов на ВМА +-255

  int deltaForce = VMA_MAX_SPEED * 5.12;
  if (deltaForce == 0) {
    deltaForce = 1; 
  }
  for (int i = 0; i <= 3; i++) {
    int signedVmaValue = horVmaOutputArray[i] * (2 * horVmaDirectionArray[i] - 1);
    if (sign(signedVmaValue) * sign(oldSignedVmaArray[i]) < 0) {
      signedVmaValue = 0;
    } 
    else if (sign(signedVmaValue)*(signedVmaValue - oldSignedVmaArray[i]) > deltaForce) {
      signedVmaValue = oldSignedVmaArray[i] + deltaForce * sign(signedVmaValue - oldSignedVmaArray[i]);
    }
    if (signedVmaValue >= 0) {
      horVmaDirectionArray[i] = 1;
    } 
    else {
      horVmaDirectionArray[i] = 0;
    }
    horVmaOutputArray[i] = abs(signedVmaValue);
    oldSignedVmaArray[i] = signedVmaValue;
  }
}


int sign (int number) 
{
  if (number == 0) {
    return 0;
  } 
  else {
    return number/abs(number); 
  }
}

