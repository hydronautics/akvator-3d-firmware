#define SENSOR_VLF_PIN  2
#define SENSOR_VLB_PIN  7
#define SENSOR_VRB_PIN  1
#define SENSOR_VRF_PIN  0


#define VLF_DIR_PIN  4
#define VLB_DIR_PIN  6
#define VRB_DIR_PIN  12
#define VRF_DIR_PIN  7

#define VLF_PWM_PIN  3
#define VLB_PWM_PIN  5
#define VRB_PWM_PIN  11
#define VRF_PWM_PIN  9

//порядковые номера каждого ВМА в массивах с номерами пинов
#define VLF  0
#define VLB  1
#define VRB  2
#define VRF  3

//максимальная скорость изменения сигнала на ВМА
//измеряется в количестве полных перебросок в сек.
#define VMA_MAX_SPEED 1.3*(100/FREQUENCY)

//0 - левый передний, 1 - левый задний, 2 - правый задний, 3 - правый передний

//VLF        VRF
//0   _      _ 3
//  _____    _____
// /     \  /     \
//|   o   ||   o   |
// \_____/  \_____/
//___|_|______|_|_
//|               |
//|               |
//|               |
//|               |
//|____________ __|
//  _|_|_    _|_|_
// /     \  /     \
//|   o   ||   o   |
// \_____/  \_____/
// 1              2
//VLB        VRB
