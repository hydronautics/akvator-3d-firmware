void tools_init()
{

  //firstToolServo.attach(TOOL1_SERVO_PIN);
  //secondToolServo.attach(TOOL2_SERVO_PIN);
  cameraServo.attach(CAMERA_SERVO_PIN);
  cameraServo.write(90);

  manOneServo.attach(MAN1_PIN); // Цепляем объекты Сервок на соответствующие пины
  manOneServo.write(FIRST_SERVO_MIDDLE); // Выставляем сервы в серединку

  manTwoServo.attach(MAN2_PIN);
  manTwoServo.write(SECOND_SERVO_MIDDLE);

  manThreeServo.attach(MAN3_PIN);
  manThreeServo.write(THIRD_SERVO_MIDDLE);
  
  leftToolServo.attach(TOOL1_SERVO_PIN);
  leftToolServo.writeMicroseconds(TOOL1_MIDDLE);
  
  rightToolServo.attach(TOOL3_SERVO_PIN);
  rightToolServo.writeMicroseconds(TOOL3_MIDDLE);
  
  pinMode(LIGHT_PIN,OUTPUT);
  digitalWrite(LIGHT_PIN,LOW);

  pinMode(PUMP_PIN,OUTPUT); // Помпа изначально выключена
  digitalWrite(PUMP_PIN,LOW);

  pinMode(GRAB_DIR_PIN,OUTPUT);
  digitalWrite(GRAB_DIR_PIN,LOW);

  //pinMode(GRAB_PWM_PIN,OUTPUT);
  analogWrite(GRAB_PWM_PIN,0);
}

void tools_output(byte light,byte camera,char grab, char shoulder,char elbow,char wrist, char left_tool,char right_tool) 
//shoulder, elbow, wrist - сигналы управления каждым из звеньев, от -128 до 127.
{
  static byte lastCam; // 
  static byte temp;

  static byte cycleCount=0; // Эта переменная используется только в режиме попеременного движения звеньев.
  //static boolean startedSwitching=false;
  static boolean shoulderNotElbow=true; // Эта переменная используется только в режиме попеременного движения звеньев.

  //  static byte pumpOnCount=0;
  static int pumpOffCountDown;

  static boolean startedPumpAcceleration;



  char shoulder_speed=0; // А это уже смещение серв от серединного положения, которое пропорционально скорости движения каждого звена
  char elbow_speed=0; // Эти переменные будут отправляться на сервы, поэтому из значения - в градусах.
  char wrist_speed=0;

  static byte pump; // Величина тяги на помпу

  if (servSwitching) // Если работаем в режиме попеременного переключения звеньев
  {

    if ((shoulder!=0) && (elbow!=0))  // Проверяем, не пытаемся ли мы  двигать сразу двумя сервами Плеча и Локтя

    {
      // Сначала смотри else
      // Если да, то начинаем инкрементировать cycleCount

      if (cycleCount == (SYSTEM_CLOCK_FREQUENCY / SHOULDER_ELBOW_SWITCH_FREQ)-1) // Если с момента начала 
      {
        shoulderNotElbow = !shoulderNotElbow;
        cycleCount=0;
      }

      if (shoulderNotElbow)
      {
        shoulder_speed=map(shoulder,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
        elbow_speed=0;
      } 
      else
      {
        shoulder_speed=0;
        elbow_speed= map(elbow,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
      } 

      cycleCount++;  // Если да, то начинаем считать циклы с этого момента
    }

    else
    { // Если нет, то обнуляем cycleCount
      cycleCount=0;
      // Записываем значения shoulder_speed и elbow_speed. 
      // Одно из них гарантированно ноль. Это нам гарантирует самый первый if(shoulder!=0) && (elbow!=0)
      shoulder_speed=map(shoulder,-128,127,-SERVO_LIMIT,SERVO_LIMIT); 
      elbow_speed=map(elbow,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
      // Звено кисти получает свое значение независимо от того, какой включен режим движения и независимо от того какие сервы еще движутся.
      // Просто потому что у него отдельная кнопка и им не получится рулить одновременно  с другими звеньями.
    }
  }

  else
    // Если включен режим одновременного движения сервок, то просто высчитываем их значения.

  {
    shoulder_speed=map(shoulder,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
    elbow_speed=map(elbow,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
  }
  wrist_speed = map(wrist,-128,127,-SERVO_LIMIT,SERVO_LIMIT); // Звено кисти получает свое значение независимо от того, какой включен режим движения и независимо от того какие сервы еще движутся.



  if (((shoulder_speed!=0) && (elbow_speed!=0))||((shoulder_speed!=0) && (wrist_speed!=0))||((wrist_speed!=0) && (elbow_speed!=0)))  // Если двигаем двумя или тремя сервами, 
  {
    if ( pump != (INCREASED_PUMP_VOLTAGE*255)/POWER_VOLTAGE)
      pump++;

    pumpOffCountDown=0;

  }
  else

      if ((0==shoulder_speed)&&(0==elbow_speed)&&(0==wrist_speed)) // если подаем нули
    {

      pumpOffCountDown++;
      if (pumpOffCountDown == (SYSTEM_CLOCK_FREQUENCY * PUMP_OFF_DELAY))
      {
        pump=0;
        pumpOffCountDown=0; 
        startedPumpAcceleration=false;
      }
    }

    else // если рулим только одной сервой
    {
      if (pump != (NOMINAL_PUMP_VOLTAGE*255)/POWER_VOLTAGE)
        pump++;
      pumpOffCountDown=0;


    }




  if (fullStop)
    pump=0;


  analogWrite(PUMP_PIN,pump);
  manOneServo.write(FIRST_SERVO_MIDDLE + shoulder_speed);
  manTwoServo.write(SECOND_SERVO_MIDDLE + elbow_speed);
  manThreeServo.write(THIRD_SERVO_MIDDLE - wrist_speed); // В другую сторону!!!



  analogWrite(LIGHT_PIN,light);

  //  pump = (ONE_JOINT_POWER*255)/POWER_VOLTAGE;



  if (grab>0)
    digitalWrite(GRAB_DIR_PIN,HIGH);
  else
    digitalWrite(GRAB_DIR_PIN,LOW);

  analogWrite(GRAB_PWM_PIN,map(abs(grab),0,128,0,255));


  if (lastCam!=camera)
  {
    temp=constrain(map(camera,0,255,0,180),CAM_ANGLE_LOWER,CAM_ANGLE_UPPER);
    cameraServo.write(temp);

  }
  
  leftToolServo.writeMicroseconds(TOOL1_MIDDLE + left_tool);
  rightToolServo.writeMicroseconds(TOOL3_MIDDLE + right_tool);
  
  
  
  if (controlDebug)
  {
    if (manDebug)
    {
      Serial.print (" \n\tShoulder ");
      Serial.print(shoulder_speed,DEC);
      Serial.print (" Elbow ");
      Serial.print(elbow_speed,DEC);
      Serial.print (" Wrist ");
      Serial.print(wrist_speed,DEC);

      Serial.print (" CycleCount ");
      Serial.print(cycleCount);

      Serial.print (" ShoulderNotElbow ");
      Serial.print(shoulderNotElbow);

      Serial.print (" PumpOffCountDown ");
      Serial.print(pumpOffCountDown);

    }
    else
    {
      Serial.print("\n\tLIGHT OUT ");
      Serial.print(light);
      Serial.print (" CAM OUT ");
      Serial.print(temp);
    }

    Serial.print (" PUMP OUT ");
    Serial.println(pump);

  }

  lastCam=camera;
}




void man_test_control()
{
  byte pump=0;
  
  char shoulder = 0;
  
  char elbow = 0;
  
  char wrist = 0;
  
  if (fullStop)
    pump=0;
    else
    pump = (NOMINAL_PUMP_VOLTAGE*255)/POWER_VOLTAGE;
    
    shoulder = map(manOneSpeed,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
    
    elbow = map(manTwoSpeed,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
    wrist = map(manThreeSpeed,-128,127,-SERVO_LIMIT,SERVO_LIMIT);
    
    
  
  analogWrite(PUMP_PIN,pump);
  manOneServo.write(FIRST_SERVO_MIDDLE+shoulder);
  manTwoServo.write(SECOND_SERVO_MIDDLE+elbow); 
  manThreeServo.write(THIRD_SERVO_MIDDLE+wrist);

  Serial.print(shoulder,DEC);
  Serial.print('\t');
  Serial.print(elbow,DEC);
  Serial.print('\t');
  Serial.print(wrist,DEC);
  
  
  Serial.print(" \tPump ");
  Serial.println(pump,DEC);
}









