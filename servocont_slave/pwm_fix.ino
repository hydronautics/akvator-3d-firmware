void pwm_fix()
{
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10); 
  // _BV(COM1A1) - D11 прямой ШИМ                _BV(COM1B1) - D12 прямой ШИМ
  // _BV(COM1A1)|_BV(COM1A0) - D11 инвесный ШИМ  _BV(COM1B1)|_BV(COM1B0) - D12 инвесный ШИМ
  // _BV(WGM10) - Fast PWM
  // _BV(WGM10) - Phase Correct PWM
  
  TCCR1B = _BV(CS10);
  // _BV(WGM12) - Fast PWM
  // _BV(CS10) - делитель 1
  // _BV(CS11) - делитель 8
  // _BV(CS11) | _BV(CS10) - делитель 64
  // _BV(CS12) - делитель 256
  // _BV(CS12) | _BV(CS10) - делитель 1024 
  
  // Инициализация ШИМ на OCR2A - D10
}
