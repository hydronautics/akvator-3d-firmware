#define SENSOR_HLF_PIN  0
#define SENSOR_HLB_PIN  2
#define SENSOR_HRB_PIN  3
#define SENSOR_HRF_PIN  1



#define HLF_DIR_PIN   7
#define HLB_DIR_PIN  12
#define HRB_DIR_PIN  8
#define HRF_DIR_PIN  10

#define HLF_PWM_PIN  5
#define HLB_PWM_PIN  11
#define HRB_PWM_PIN  9
#define HRF_PWM_PIN  6

#define HLF  0
#define HLB  1
#define HRB  2
#define HRF  3

//максимальная скорость изменения сигнала на ВМА
//измеряется в количестве полных перебросок в сек.
#define VMA_MAX_SPEED 3*(100/FREQUENCY)


//0 - левый передний, 1 - левый задний, 2 - правый задний, 3 - правый передний



//HLF        HRF
//0   _      _ 3
//   / |    | \
//   //      \\
// ______________
//|              |
//|              |
//|              |
//|              |
//|______________|
//
//   \\      //
//1  \_|    |_/ 2  
//HLB        HRB
