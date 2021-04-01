#include "BSP.h"
#include "GUI/DisplayPrivate.h"
#include "rtc.h"

/*�ڽ��͹���ģʽʱ��������Ϊ���͵�����*/
static const uint8_t PinGrpLow[] =
{
    TFT_CS_Pin,
    TFT_DC_Pin,
    TFT_RST_Pin,
    TFT_BLK_Pin,
    
    LED_Pin,

    KEY_UP_Pin,
//    KEY_OK_Pin,   //ʹ��OK������
    KEY_DOWN_Pin
};

/*�ڽ��͹���ģʽʱ��������Ϊ�������������*/
static const uint8_t PinGrpFloating[] =
{
    IMU_SCL_Pin,
    IMU_SDA_Pin,
    IMU_INT_Pin,
    BMP_SCL_Pin,
    BMP_SDA_Pin,
    BAT_DET_Pin,
    BAT_CHG_Pin
};

/**
  * @brief  �͹��Ļ����¼������ⲿ�жϴ���
  * @param  ��
  * @retval ��
  */
static void Power_AwakeCallback()
{
    digitalWrite(LED_Pin, HIGH);
    NVIC_SystemReset();           //�������
}

/**
  * @brief  ����͹���ģʽ
  * @param  ��
  * @retval ��
  */
static void Power_EnterLowPowerMode()
{
    /*�������ţ�����ģʽ*/
    for(uint8_t i = 0; i < sizeof(PinGrpLow); i++)
    {
        pinMode(PinGrpLow[i], OUTPUT);
        digitalWrite(PinGrpLow[i], LOW);
    }
    for(uint8_t i = 0; i < sizeof(PinGrpFloating); i++)
    {
        pinMode(PinGrpFloating[i], INPUT);
    }
    
    /*������*/
    ADC_Cmd(ADC1, DISABLE);
    ADC_Cmd(ADC2, DISABLE);
    DMA_Cmd(DMA2_Stream0, DISABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    TIM_Cmd(TIM3, DISABLE);
    TIM_Cmd(TIM4, DISABLE);
    
    /*�ȴ��ɿ���Դ��*/
    while (digitalRead(POWER_EN_Pin)) __NOP();
    digitalWrite(POWER_ON_Pin, LOW);

    /*ע���ⲿ�жϻص�*/
    attachInterrupt(KEY_OK_Pin, Power_AwakeCallback, FALLING);
    
    /*��STOPģʽ*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
    while(1);
}

/*��һ�β���ʱ��(ms)*/
static uint32_t Power_LastHandleTime = 0;

/*�Զ��ػ�ʱ��(��)*/
static uint16_t Power_AutoLowPowerTimeout = 60;

/*�Զ��ػ�����ʹ��*/
static bool Power_AutoLowPowerEnable = true;

/**
  * @brief  �Ӻ󱸼Ĵ�����ȡ�Զ��ػ�ʱ��
  * @param  ��
  * @retval ʱ��(��)
  */
static uint32_t Power_GetBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //ʹ��PWR��BKP����ʱ��
    PWR_BackupAccessCmd(ENABLE);                          //ʹ�ܺ󱸼Ĵ�������
    
    uint16_t value = RTC_ReadBackupRegister(RTC_BKP_DR3);
    
    if(value == 0 || value > 300)
    {
        value = 60;
    }
    
    return value;
}

/**
  * @brief  ����ǰ�Զ��ػ�ʱ��������󱸼Ĵ���
  * @param  ��
  * @retval ��
  */
static void Power_UpdateBKP()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);   //ʹ��PWR��BKP����ʱ��
    PWR_BackupAccessCmd(ENABLE);  //ʹ�ܺ󱸼Ĵ�������
    RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_WriteBackupRegister(RTC_BKP_DR3, Power_GetAutoLowPowerTimeout()); //д��󱸼Ĵ���
    RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
}

/**
  * @brief  ��Դ��ʼ��
  * @param  ��
  * @retval ��
  */
void Power_Init()
{
    /*��Դ����*/
    pinMode(POWER_ON_Pin, OUTPUT);
    digitalWrite(POWER_ON_Pin, HIGH);
    
    /*��Դ����*/
    pinMode(POWER_EN_Pin, INPUT_PULLDOWN);
    
    /*��س����*/
    pinMode(BAT_CHG_Pin, INPUT_PULLUP);
    
    /*��ص�ѹ���*/
    pinMode(BAT_DET_Pin, INPUT_ANALOG_DMA);
    
    Power_SetAutoLowPowerTimeout(Power_GetBKP());
    Power_HandleTimeUpdate();
}

/**
  * @brief  ���²���ʱ��
  * @param  ��
  * @retval ��
  */
void Power_HandleTimeUpdate()
{
    Power_LastHandleTime = millis();
}

/**
  * @brief  �����Զ��ػ�ʱ��
  * @param  sec:ʱ��(��)
  * @retval ��
  */
void Power_SetAutoLowPowerTimeout(uint16_t sec)
{
    Power_AutoLowPowerTimeout = sec;
}

/**
  * @brief  ��ȡ�Զ��ػ�ʱ��
  * @param  ��
  * @retval sec:ʱ��(��)
  */
uint16_t Power_GetAutoLowPowerTimeout()
{
    return Power_AutoLowPowerTimeout;
}

/**
  * @brief  �����Զ��ػ�����ʹ��
  * @param  en:ʹ��
  * @retval ��
  */
void Power_SetAutoLowPowerEnable(bool en)
{
    Power_AutoLowPowerEnable = en;
    Power_HandleTimeUpdate();
}

/**
  * @brief  ִ�йػ�
  * @param  ��
  * @retval ��
  */
void Power_Shutdown()
{
    Backlight_UpdateBKP();
    Backlight_SetGradual(0, 500);
    Power_UpdateBKP();
    PageDelay(500);
    Power_EnterLowPowerMode();
}

/**
  * @brief  �Զ��ػ����
  * @param  ��
  * @retval ��
  */
void Power_AutoShutdownUpdate()
{
    if(!Power_AutoLowPowerEnable)
        return;
    
    if(Power_AutoLowPowerTimeout == 0)
        return;
    
    if(millis() - Power_LastHandleTime >= (Power_AutoLowPowerTimeout * 1000))
    {
        Power_Shutdown();
    }
}
