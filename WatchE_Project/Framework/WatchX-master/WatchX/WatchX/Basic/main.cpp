#include "BSP/BSP.h"
#include "GUI/DisplayPrivate.h"
#include "MillisTaskManager/MillisTaskManager.h"

/*���������*/
static MillisTaskManager mtmMain;

/*CPUռ����(DEBUG��)*/
float CPU_Usage;

/**
  * @brief  CPUռ���ʸ���
  * @param  ��
  * @retval ��
  */
static void CPU_UsageUpdate()
{
    CPU_Usage = mtmMain.GetCPU_Usage();
}

/**
  * @brief  ϵͳ��ʼ��
  * @param  ��
  * @retval ��
  */
static void setup()
{
    LED_Init();
    LED_SetEnable(true);

    /*�豸��ʼ��*/
    Power_Init();
    RTCx_Init();
    Backlight_Init();
    Button_Init();
    Display_Init();
    BMP_Init();
    ADC_DMA_Init();

    LED_SetEnable(false);

    /*����ע��*/
    mtmMain.Register(Display_Update, 1);                //��Ļˢ��
    mtmMain.Register(Button_Update, 10);                //�����¼����
    mtmMain.Register(Power_AutoShutdownUpdate, 100);    //�Զ��ػ����
    mtmMain.Register(CPU_UsageUpdate, 1000);            //CPUռ���ʼ��
}

/**
  * @brief  ϵͳ��ѭ��
  * @param  ��
  * @retval ��
  */
static void loop()
{
    mtmMain.Running(millis());
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    Delay_Init();
    setup();
    for(;;)loop();
}
