#include "IMU_VN100.h"
#include <avr/interrupt.h>
#include "Arduino.h"


// Constructor ////////////////////////////////////////////////////////////////
IMU_Class::IMU_Class()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void IMU_Class::Init(long baudrate)
{
  IMU_checksum_calc = false;
  bufferidx = 0;
  NewData = 0;
  NewYPR = 0;
  NewAccel = 0;
  NewAng = 0;


  Serial3.begin(baudrate);					// Initialize serial port
}


void IMU_Class::Read(void)
{
  char c;
  //int numc;
  //int i;
  // numc = Serial3.available();  
	 //Serial.println(Serial3.available());

  // if (numc > 0)
    // for (i=0; i<numc; i++)
	while (Serial3.available())
    {
      c = Serial3.read();   
      if (c == '$')						// IMU Serial Start
      {                      
        bufferidx = 0;
        buffer[bufferidx++] = c;
        IMU_checksum = 0;
        IMU_checksum_calc = true;
        continue;
      }
      if (c == '\r')						// IMU Serial End
      {                     
        buffer[bufferidx++] = 0;
        parse_IMU();
		//break;
      }
      else 
      {
        if (bufferidx < (IMU_BUFFERSIZE-1))
        {
          if (c == '*')
            IMU_checksum_calc = false;    // Checksum calculation end
          buffer[bufferidx++] = c;
          if (IMU_checksum_calc)
            IMU_checksum ^= c;            // XOR 
        }
        else
          bufferidx=0;   // Buffer overflow : restart
      }
    }   
}

/****************************************************************
 * 																*
 ****************************************************************/
// Private Methods //////////////////////////////////////////////////////////////
void IMU_Class::parse_IMU(void)
{
  byte IMU_check;
  byte XStemp, YStemp, ZStemp;
  float Upper,Lower;
  char *parseptr;									// pointer to parser
  byte Offset=0;


  /*********************************
   * IMU Roll, Pitch, Yaw Parser    *
   **********************************/

  if ((strncmp(buffer,"$VNYPR",6)==0) || (strncmp(buffer,"$VNRRG,08",9)==0))				// Check if sentence begins with $VNYPR    [GPGGA]
  {   

	if (strncmp(buffer,"$VNYPR",6)==0)
		Offset = 7;
	else
		Offset = 10;
		
    if (buffer[bufferidx-4]=='*')					// Check for the "*" character
    {          		
      IMU_check = parseHex(buffer[bufferidx-3])*16 + parseHex(buffer[bufferidx-2]);    // Read the checksums characters
      if (IMU_checksum == IMU_check)				// Checksum validation
      {      							
        XStemp = buffer[Offset];						// Yaw Sign (+ or -)
        YStemp = buffer[Offset+9];					// Pitch Sign (+ or -)
        ZStemp = buffer[Offset+18];					// Roll Sign (+ or -)
		
        if (XStemp == 45)						// if equal to - then set modifier to negative
          YawSign = -1;
        else
          YawSign = 1;					// else leave positive
        if (YStemp == 45)						// if equal to - then set modifier to negative
          PitchSign = -1;
        else
          PitchSign = 1;					// else leave positive
        if (ZStemp == 45)						// if equal to - then set modifier to negative
          RollSign = -1;
        else
          RollSign = 1;					// else leave positive
		  
		if (Offset == 10)
			parseptr = strchr(buffer, ',')+5;			// look for comma-seperator after the $VNYPR identifier
		else
			parseptr = strchr(buffer,',')+2;

        Upper = (float)decimalParser(parseptr,3);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);			// get lower (after decimal) numbers
        Yaw = ((float)YawSign)*((Upper) + (Lower/1000.0));			// join values together to get whole reading
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,3);
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);      		// Pitch
        Pitch = ((float)PitchSign)*((Upper) + (Lower/1000.0));
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,3);
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);			// Roll
        Roll = ((float)RollSign)*((Upper) + (Lower/1000.0));
		
		NewYPR = 1;  // New IMU Data				// Flag when new data is available

      }
      
	  
	  else
      {
        if (PrintErrors)
          Serial.println("IMU ERROR: Checksum error!!");
      }
	  }
	  else
	  {
	  if (PrintErrors)
		Serial.println("IMU ERROR: Checksum Read error!!");
	  }
    
	}
  
   else if ((strncmp(buffer,"$VNACC",6)==0) || (strncmp(buffer,"$VNRRG,18",9)==0))				// Check if sentence begins with $VNACC    [GPGGA]
  {   

	if (strncmp(buffer,"$VNACC",6)==0)
		Offset = 7;
	else
		Offset = 10;
		
    if (buffer[bufferidx-4]=='*')					// Check for the "*" character
    {          		
      IMU_check = parseHex(buffer[bufferidx-3])*16 + parseHex(buffer[bufferidx-2]);    // Read the checksums characters
      if (IMU_checksum == IMU_check)				// Checksum validation
      {      							
        XStemp = buffer[Offset];						// Accel X Sign (+ or -)
        YStemp = buffer[Offset+8];					// Accel Y Sign (+ or -)
        ZStemp = buffer[Offset+16];					// Accel Z Sign (+ or -)
		
        if (XStemp == 45)						// if equal to - then set modifier to negative
          accelSign[X] = -1;
        else
          accelSign[X] = 1;					// else leave positive
        if (YStemp == 45)						// if equal to - then set modifier to negative
          accelSign[Y] = -1;
        else
          accelSign[Y] = 1;					// else leave positive
        if (ZStemp == 45)						// if equal to - then set modifier to negative
          accelSign[Z] = -1;
        else
          accelSign[Z] = 1;					// else leave positive

        if (Offset == 10)
			parseptr = strchr(buffer, ',')+5;			// look for comma-seperator after the $VNYPR identifier
		else
			parseptr = strchr(buffer,',')+2;

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);			// get lower (after decimal) numbers
        Acceleration[X] = ((float)accelSign[X])*((Upper) + (Lower/1000.0));			// join values together to get whole reading
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);			// get lower (after decimal) numbers
        Acceleration[Y] = ((float)accelSign[Y])*((Upper) + (Lower/1000.0));			// join values together to get whole reading
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,3);			// get lower (after decimal) numbers
        Acceleration[Z] = ((float)accelSign[Z])*((Upper) + (Lower/1000.0));			// join values together to get whole reading
		
		NewAccel = 1;  // New IMU Data				// Flag when new data is available

      }
      
	  
	  else
      {
        if (PrintErrors)
          Serial.println("IMU ERROR: Checksum error!!");
      }
	  
	  }
	  else
	  {
	  if (PrintErrors)
		Serial.println("IMU ERROR: Checksum Read error!!");
	  }
    
	}
  	
  else if ((strncmp(buffer,"$VNGYR",6)==0) || (strncmp(buffer,"$VNRRG,19",9)==0))				// Check if sentence begins with $VNACC    [GPGGA]
  {   

	if (strncmp(buffer,"$VNGYR",6)==0)
		Offset = 7;
	else
		Offset = 10;
		
    if (buffer[bufferidx-4]=='*')					// Check for the "*" character
    {          		
      IMU_check = parseHex(buffer[bufferidx-3])*16 + parseHex(buffer[bufferidx-2]);    // Read the checksums characters
      if (IMU_checksum == IMU_check)				// Checksum validation
      {      							
        XStemp = buffer[Offset];						// Angul.Rate X Sign (+ or -)
        YStemp = buffer[Offset+11];					// Angul.Rate Y Sign (+ or -)
        ZStemp = buffer[Offset+22];					// Angul.Rate Z Sign (+ or -)
		
        if (XStemp == 45)						// if equal to - then set modifier to negative
          angSign[X] = -1;
        else
          angSign[X] = 1;					// else leave positive
        if (YStemp == 45)						// if equal to - then set modifier to negative
          angSign[Y] = -1;
        else
          angSign[Y] = 1;					// else leave positive
        if (ZStemp == 45)						// if equal to - then set modifier to negative
          angSign[Z] = -1;
        else
          angSign[Z] = 1;					// else leave positive

        if (Offset == 10)
			parseptr = strchr(buffer, ',')+5;			// look for comma-seperator after the $VNYPR identifier
		else
			parseptr = strchr(buffer,',')+2;

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,4);			// get lower (after decimal) numbers
        Angular[X] = ((float)angSign[X])*((Upper) + (Lower/10000.0));			// join values together to get whole reading
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,4);			// get lower (after decimal) numbers
        Angular[Y] = ((float)angSign[Y])*((Upper) + (Lower/10000.0));			// join values together to get whole reading
        parseptr = strchr(parseptr, ',')+2;			// look for comma-seperator

        Upper = (float)decimalParser(parseptr,2);        	// get upper (before decimal) numbers
        parseptr = strchr(parseptr, '.')+1;			// look for decimal-seperator
        Lower = (float)decimalParser(parseptr,4);			// get lower (after decimal) numbers
        Angular[Z] = ((float)angSign[Z])*((Upper) + (Lower/10000.0));			// join values together to get whole reading
		
		NewAng = 1;  // New IMU Data				// Flag when new data is available

      }
      
	  
	  else
      {
        if (PrintErrors)
          Serial.println("IMU ERROR: Checksum error!!");
      }
	  
	  }
	  else
	  {
	  if (PrintErrors)
		Serial.println("IMU ERROR: Checksum Read error!!");
	  }
    
	}
  
  else
  {
    
    if (PrintErrors)
    {
      Serial.println("IMU_ERROR: Bad sentence!");
      for (int i=0;i<bufferidx;i++)
        	Serial.print(buffer[i]);
      Serial.println();
    }
	
	bufferidx = 0;
  }

}

/****************************************************************
 * 
 ****************************************************************/
// Parse hexadecimal numbers
byte IMU_Class::parseHex(char c)
{
  if (c < '0')
    return (0);
  if (c <= '9')
    return (c - '0');
  if (c < 'A')
    return (0);
  if (c <= 'F')
    return ((c - 'A')+10);
	else
	return (0);
}

// Распознает число из numdec цифр, начиная с элемента строки, указанного через *str
unsigned int IMU_Class::decimalParser(char *str,byte numdec)
{
	int temp=0;
	for (int i=0;i<numdec;i++)
	{
		temp*=10;
		temp+=str[i]-'0';
	}
	return temp;
}

IMU_Class IMU; 
