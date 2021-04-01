#include "BSP.h"
#include "Wire.h"

/*ʵ�������I2C���󣬿���ģʽ*/
TwoWire WireBMP(BMP_SCL_Pin, BMP_SDA_Pin, SOFT_FAST);

/*ʵ������ѹ�ƶ���*/
Adafruit_BMP085 BMP180;

/**
  * @brief  ��ѹ�Ƴ�ʼ��
  * @param  ��
  * @retval ��
  */
void BMP_Init()
{
    while(!BMP180.begin());
}

/**
  * @brief  ��ѹ��״̬����
  * @param  ��
  * @retval ��
  */
void BMP_Update()
{
    BMP180.updateAll();
}
