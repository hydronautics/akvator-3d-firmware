const byte sdata =8;
const byte  sclk= 9;
const byte  busy = 7;
const byte  convst = 6;


//const float ADCstep = 1.22; // цена деления ацп в милливольтах
const byte signBit = 5; // номер знакового бита в старшем недобайте 14-разрядного числа



void ADC_init()
{
   pinMode(sdata,INPUT);

  pinMode(sclk,OUTPUT);
  digitalWrite(sclk,LOW);

  pinMode(busy,INPUT);

  pinMode(convst,OUTPUT);
  digitalWrite(convst,HIGH);
  
}

int ADC_read()
{
  
  byte low=0; // младший байт 14 разрядного числа
  byte high =0; // 2 нуля + старшие 6 бит 14-разрядного числа
  
  int intResult = 0;

  digitalWrite(convst,LOW); 
  digitalWrite(convst,HIGH); // Старт конверсии
  
  // Ждать сигнала busy не нужно
  // Экспериментально выяснено, что к этому моменту данные уже готовы.

  high = shiftIn(sdata,sclk,MSBFIRST); // Считываем старший байт 
  low = shiftIn(sdata,sclk,MSBFIRST); // Младший байт

 if (bitRead(high,signBit)==1)
    {
     bitClear(high,signBit); // Сбрасываем знаковый бит в 0 для 14-разрядного числа
     intResult =  (word(high,low) * (-1));
    }
    else
    intResult = word(high,low);//результат в разрядах
    
    if (adcDebug)
      Serial.println(intResult);
      
      return intResult;
}
    


