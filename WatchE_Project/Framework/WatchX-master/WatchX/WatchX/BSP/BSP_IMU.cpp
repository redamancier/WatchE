#include "BSP.h"
#include "Wire.h"

/*ʵ�������I2C���󣬿���ģʽ*/
TwoWire Wire(IMU_SCL_Pin, IMU_SDA_Pin, SOFT_FAST);
