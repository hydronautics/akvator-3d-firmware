#define DEPTH_PIN A3

const int ADC_POLL_FREQUENCY =  300; //частота семплирования АЦП
const int depth_tau = (1000/ADC_POLL_FREQUENCY); // период квантования АЦП, в миллисекундах

const float depth_const_T = 500; // постоянная времени апериодического звена по выходу ацп, в миллисекундах
const float depth_K = 1.0;
const float depth_const_A =  (depth_const_T)/((depth_const_T + (float)depth_tau));
const float depth_const_B = depth_K*((float)depth_tau)/ (depth_const_T + (float)depth_tau);

const float speed_const_T = 100; // постоянная времени апериодического звена по выходу ацп, в миллисекундах
const float speed_K = 1.0;
const float speed_const_A =  (speed_const_T)/((speed_const_T + (float)depth_tau));
const float speed_const_B = speed_K*((float)depth_tau)/ (speed_const_T + (float)depth_tau);



//#define NUMBER_OF_MEASURES 20
//
//#define TO_METERS_CONVERSION 0.015

//int depthMeasures[NUMBER_OF_MEASURES];

float rawDepth = 0;
int oldDepth = 0; // -128 ... 127
float rawVelocity=0;
float lastRawDepth=0;
float filteredVelocity=0;

int intDepthVelocity = 0;

float filteredDepth = 0;

void getDepth()
{
  
  rawDepth = ADC_read() * DEPTH_SCALE_ADC;
  filteredDepth = depth_const_A*filteredDepth + depth_const_B*rawDepth;
  
  rawVelocity = lastRawDepth - rawDepth;
  
 // filteredVelocity = speed_const_A*filteredVelocity + speed_const_B*rawVelocity;
  filteredVelocity = rawVelocity;
  
  lastRawDepth = rawDepth;
}
