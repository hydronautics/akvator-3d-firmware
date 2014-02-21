// В контроллере горизонтальных ВМА используются ШИМ-пины под номерами 5,6, 9 и 11
void hor_pwm_fix()
{
     // Инициализация ШИМ на OCR2A - D11  
  
  TCCR2A = _BV(COM2A1) | _BV(WGM20); 
  // _BV(COM2A1) - D11 прямой ШИМ                _BV(COM2B1) - D3 прямой ШИМ
  // _BV(COM2A1)|_BV(COM2A0) - D11 инвесный ШИМ  _BV(COM2B1)|_BV(COM2B0) - D3 инвесный ШИМ
  // _BV(WGM21) | _BV(WGM20) - Fast PWM
  // _BV(WGM20) - Phase Correct PWM
  
  TCCR2B = _BV(CS20);
  // _BV(CS20) - делитель 1
  // _BV(CS21) - делитель 8
  // _BV(CS21) | _BV(CS20) - делитель 32
  // _BV(CS22) - делитель 64
  // _BV(CS22) | _BV(CS20) - делитель 128
  // _BV(CS22) | _BV(CS21) - делитель 256
  // _BV(CS22) | _BV(CS21) | _BV(CS20) - делитель 1024 
  
    // Инициализация ШИМ на OCR2A - D9 и OCR2B - D10 

  
  TCCR1A = _BV(COM1A1) | _BV(WGM10); 
  // _BV(COM1A1) - D9 прямой ШИМ                 _BV(COM1B1) - D10 прямой ШИМ
  // _BV(COM1A1)|_BV(COM1A0) - D9 инвесный ШИМ   _BV(COM1B1)|_BV(COM1B0) - D10 инвесный ШИМ
  // _BV(WGM12) | _BV(WGM10) - Fast PWM 8-bit
  // _BV(WGM12) | _BV(WGM11) - Fast PWM 9-bit
  // _BV(WGM12) | _BV(WGM11) | _BV(WGM10) - Fast PWM 10-bit
  // _BV(WGM10) - Phase Correct PWM 8-bit
  // _BV(WGM11) - Phase Correct PWM 9-bit
  // _BV(WGM11) | _BV(WGM10) - Phase Correct PWM 10-bit
  
  TCCR1B = _BV(CS10);
  // _BV(CS10) - делитель 1
  // _BV(CS11) - делитель 8
  // _BV(CS11) | _BV(CS10) - делитель 64
  // _BV(CS12) - делитель 256
  // _BV(CS12) | _BV(CS10) - делитель 1024

  // Инициализация ШИМ на OCR2A - D6 и OCR2B - D5 
  
  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00); 
  // _BV(COM0A1) - D6 прямой ШИМ                 _BV(COM0B1) - D5 прямой ШИМ
  // _BV(COM0A1)|_BV(COM0A0) - D6 инвесный ШИМ   _BV(COM0B1)|_BV(COM2B0) - D5 инвесный ШИМ
  // _BV(WGM01) | _BV(WGM00) - Fast PWM
  // _BV(WGM00) - Phase Correct PWM
  
  TCCR0B = _BV(CS00);
  // _BV(CS00) - делитель 1
  // _BV(CS01) - делитель 8
  // _BV(CS01) | _BV(CS00) - делитель 64
  // _BV(CS02) - делитель 256
  // _BV(CS02) | _BV(CS00) - делитель 1024 
  
  } 
  

