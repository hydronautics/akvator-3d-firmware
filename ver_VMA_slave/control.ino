byte directionPin[4];
byte pwmPin[4];

int verVmaOutputArray[4];
boolean verVmaDirectionArray[4];

int current_VLB_null;
int current_VLF_null;
int current_VRF_null;
int current_VRB_null;

void control_init()
{

  directionPin[VLF]=VLF_DIR_PIN;
  directionPin[VLB]=VLB_DIR_PIN;
  directionPin[VRB]=VRB_DIR_PIN;
  directionPin[VRF]=VRF_DIR_PIN;

  pwmPin[VLF]=VLF_PWM_PIN;
  pwmPin[VLB]=VLB_PWM_PIN;
  pwmPin[VRB]=VRB_PWM_PIN;
  pwmPin[VRF]=VRF_PWM_PIN;


  for (int i = 0; i<=3;i++)
  {
    pinMode(directionPin[i],OUTPUT);
    pinMode(pwmPin[i],OUTPUT);
    digitalWrite(pwmPin[i],LOW);
    verVmaOutputArray[i]=0;
    verVmaDirectionArray[i]=true;
  }
  
  pwm_output();

  current_VLB_null=analogRead(SENSOR_VLB_PIN);
  current_VLF_null=analogRead(SENSOR_VLF_PIN);
  current_VRF_null=analogRead(SENSOR_VRF_PIN);
  current_VRB_null=analogRead(SENSOR_VRB_PIN);
  
  Serial.println("Current sensors nulls are:");
  Serial.print("VLF "); Serial.println(current_VLF_null);
  Serial.print("VLB "); Serial.println(current_VLB_null);
  Serial.print("VRB "); Serial.println(current_VRB_null);
  Serial.print("VRF "); Serial.println(current_VRF_null);
}

void distribute_vertical_output_char_Z_Roll_Pitch(char rawDepthInput, char rawRollInput, char rawPitchInput)
{
  
  if((rawDepthInput==0) && (timeDelayGlobal>SUPERDELAY))
  {
  verVmaOutputArray[VLF]=-error_speed_glub + error_speed_kren + error_speed_diff;
  verVmaOutputArray[VLB]=-error_speed_glub + error_speed_kren - error_speed_diff;
  verVmaOutputArray[VRB]=-error_speed_glub - error_speed_kren - error_speed_diff;
  verVmaOutputArray[VRF]=-error_speed_glub - error_speed_kren + error_speed_diff;
  }
else
  {
  verVmaOutputArray[VLF]=rawDepthInput + error_speed_kren + error_speed_diff;
  verVmaOutputArray[VLB]=rawDepthInput + error_speed_kren - error_speed_diff;
  verVmaOutputArray[VRB]=rawDepthInput - error_speed_kren - error_speed_diff;
  verVmaOutputArray[VRF]=rawDepthInput - error_speed_kren + error_speed_diff;
  }
  //Serial.println(verVmaOutputArray[VLF],DEC);

  //вычисление наибольшего по результатам расчёта значения тяги  
  int maxValue = 0; 
  for (int i = 0; i <= 3; i++)           
    maxValue = max(maxValue,abs(verVmaOutputArray[i]));

  for (int i = 0; i <= 3; i++)    
  {    
    verVmaDirectionArray[i] = get_direction(verVmaOutputArray[i],i);
    if (maxValue > 127)
      verVmaOutputArray[i] = map (abs(verVmaOutputArray[i]),0,maxValue,0,255);
    else        
      verVmaOutputArray[i] = map (abs(verVmaOutputArray[i]),0,127,0,255);
  }

  wma_speed_correction(); //ограничение скорости нарастания сигналов на ВМА 

  //вывод переменных в Serial монитор
  if (controlDebug) {
    control_debug_print();
  }
}

void pwm_output() //вывод ШИМ-сигналов на ВМА
{
  
  for (int i = 0; i <= 3; i++) 
  {
    analogWrite(pwmPin[i], verVmaOutputArray[i]);
    digitalWrite(directionPin[i], verVmaDirectionArray[i]);
  }
}

boolean get_direction( int wmaValue, int wmaNumber ) 
{
  boolean forseDirection = (wmaValue >= 0);  //направление тяги ВМА, больше нуля true, меньше нуля false
  boolean evenPropeller = wmaNumber % 2;    //  для нечётных номеров ВМА = 1
  return forseDirection ^ evenPropeller;    //инверсия тяги ВМА с нечётными номерами
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
    Serial.print(verVmaDirectionArray[i]);
    Serial.print(" OUT: ");
    Serial.print(verVmaOutputArray[i]);
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
    int signedVmaValue = verVmaOutputArray[i] * (2 * verVmaDirectionArray[i] - 1);
    if (sign(signedVmaValue) * sign(oldSignedVmaArray[i]) < 0) {
      signedVmaValue = 0;
    } else if (sign(signedVmaValue)*(signedVmaValue - oldSignedVmaArray[i]) > deltaForce) {
      signedVmaValue = oldSignedVmaArray[i] + deltaForce * sign(signedVmaValue - oldSignedVmaArray[i]);
    }
    if (signedVmaValue >= 0) {
      verVmaDirectionArray[i] = 1;
    } else {
      verVmaDirectionArray[i] = 0;
    }
    verVmaOutputArray[i] = abs(signedVmaValue);
    oldSignedVmaArray[i] = signedVmaValue;
  }
}


int sign (int number) {
  if (number == 0) {
    return 0;
  } else {
    return number/abs(number); 
  }
}
