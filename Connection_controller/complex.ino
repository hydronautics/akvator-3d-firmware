/*
Тут будет все, что нужно для комплексирования

============= ДАННЫЕ С ДАТЧИКОВ ====================
Три угла, float, ±179.99 (ограничение реальной величины), с точностью два знака после запятой:
IMU.Yaw - угол курса
IMU.Pitch - угол дифферента
IMU.Roll - угол крена

Три угловые скорости, float, ±3 рад/с (эмпирическое ограничение), с точностью 4 знака после запятой, что, вообще говоря, слишком много
ограничение на размер переменной ± 3.2767 рад/c
IMU.Angular[X] - скорость крена
IMU.Angular[Y] - скорость дифферента
IMU.Angular[Z] - скорость курса
ВНИМАНИЕ!!! На ВМА эти величины передаются с такой же точностью (4 знака)

Три линейных ускорения, float, ±30 м/с^2 (эмпирическое ограничение), с точностью 3 знака после запятой
IMU.Acceleration[X] - ускороение по оси X (вперед-назад)
IMU.Acceleration[Y] - ускорение по оси Y (вправо-влево)
IMU.Acceleration[Z] - ускороение по оси Z (вверх-вниз)
ВНИМАНИЕ!!! Эти величины НЕ передаются на контроллеры ВМА !!!!
*/
//Значения глубины и скорости погружения, полученные с датчика глубины
int feedbackDepth;
int feedbackDepthSpeed;

void parse_depth()
{
  feedbackDepth = word(fromDepthSensorPacket[FROM_DEPTH_HIGH],fromDepthSensorPacket[FROM_DEPTH_LOW]);
  feedbackDepthSpeed = word(fromDepthSensorPacket[FROM_DEPTH_SPEED_HIGH],fromDepthSensorPacket[FROM_DEPTH_SPEED_LOW]);
}



// =============== РЕЗУЛЬТАТЫ КОМПЛЕКСИРОВАНИЯ ================

//Значение глубины, вычисленное в рез-те комплексирования
// Передается на контроллеры ВМА в виде двух байт,
// т.е. с максимальной дискретностью в 65,536 значений на весь диапазон
float complexDepth; 

//Значение скорости погружения, вычисленное в рез-те комплексирования
// Передается на контроллеры ВМА в виде одного байта,
// т.е. с максимальной дискретностью в 256 значений на весь диапазон
float complexDepthSpeed;


//тут будет комплексирование
void complex()
{}
