// ���� ���� �������� ������������� ������� ��� ������ � ��������, ������������ � ��������� ������������ �� ����� ��������
// 1. ��������������� ������� � ����������, ������������ � ������ ��������� (�������)
// 2. ��������������� ������� � ����������, ������������ ������ � �������� ���������� � ������� ������ ����� � �� ������������ � ������ ��������� �������� (����������)
// ============== ������� ������� ==========================
//������� �������� ��� ������ packet � ������ �� � ������� ��� ������� packetSize

byte checksum_calc(const byte packet[], const byte packetSize)
{
  byte checksum=0;
  for (int i=0; i < packetSize; ++i)
  {
    checksum^=packet[i];
  }
  return checksum;
}

//-------------------------------------
//�������� �.�. � ������ packet[] �������� packetSize � ��������� �� � ����������� ������, ���������� � ��������� ������ packet[packetSize]
boolean sum_check(const byte packet[],const byte packetSize) 
{
  byte checksum=0;
  for (int i=0; i < packetSize; ++i)
  {
    checksum^=packet[i];
  };
  if (checksum == packet[packetSize])
    return true;
  else return false;
}
//----------------------------------------------

// �������� ������ packet[] �������� packetSize �� ������ address
void packet_sending(const byte address, const byte packet[],const byte packetSize)
{
  Wire.beginTransmission(address);
  Wire.write(packet,packetSize+1); // ���������� packetSize+1 ����, �.�. � ��������� ����� �������� ����������� �����
  Wire.endTransmission(); // ���������������� �������� ���������� � ���� ������.
}
//-------------------------------------------------------

// ����� � Serial-���� ������ ������� �������� ��������� ������ packet[] �������� packetSize, � ����� ����������� ����� packet[packetSize]
void packet_printing(const byte packet[],const byte packetSize)  
{
  for (int i=0; i < packetSize; ++i)
  {
    Serial.print(packet[i]);
    Serial.print("\t\t");

  }

  Serial.print("CS: ");

  Serial.println(packet[packetSize],HEX); // ����������� ����� ����� ������, ���������� � 16-������ �������
}

//------------------------------------------

void warning_print(const boolean errorCode)//0 - wrong size,1 - wrong checksum
{
  Serial.println((errorCode ? "CHECKSUM ERROR! ":"SIZE ERROR! "));
}


//---------------------------------------
void read_buffer(byte packet[])
{
  int j=0;
  while(Wire.available())
  {
    packet[j]=Wire.read();
    j++;
  }
}

