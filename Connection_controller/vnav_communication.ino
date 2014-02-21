#define YPR_REGISTER  8

#define ACCEL_REGISTER  18

#define ANG_RATES_REGISTER 19



void read_register(byte number)
{

  switch (number)
  {

  case YPR_REGISTER:
    {
      char msg[]="$VNRRG,8";
      addEnding(msg);
      Serial3.println(msg);
    }
    break;  

  case ACCEL_REGISTER:
    {
      char msg[]="$VNRRG,18";
      addEnding(msg);
      Serial3.println(msg);
    }
    break;

  case ANG_RATES_REGISTER:
    {
      char msg[]="$VNRRG,19";
      addEnding(msg);
      Serial3.println(msg);
    }
    break;   
  }
}

void addEnding(char msg[])
{
  byte checksum=0;
  int i;
  for (i=1;msg[i]!='\0';i++)
    checksum^=msg[i];
  msg[i++] = '*';
  byte temp = checksum >>4;
  if (temp <= 9)
    msg[i++] = temp + '0';
  else
    msg [i++] = temp -10 + 'A';

  temp = checksum&(B00001111);

  if (temp <= 9)
    msg[i++] = temp + '0';
  else
    msg [i++] = temp -10 + 'A';
}
