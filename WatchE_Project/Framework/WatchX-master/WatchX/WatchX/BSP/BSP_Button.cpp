#include "BSP.h"
#include "GUI/DisplayPrivate.h"

/*ʵ������������*/
ButtonEvent btUP;              //�ϼ�
ButtonEvent btOK;              //ѡ���
ButtonEvent btDOWN;            //�¼�
static ButtonEvent btPOWER;    //��Դ�������������

/**
  * @brief  �����¼��ص�����
  * @param  btn:���������ַ
  * @param  event:�¼�����
  * @retval ��
  */
static void Button_EventHandler(ButtonEvent* btn, int event)
{
    /*�Զ��ػ�ʱ�����*/
    Power_HandleTimeUpdate();
    
    /*������ҳ���¼�*/
    page.PageEventTransmit(btn, event);
}

/**
  * @brief  ��Դ�����¼��ص�����
  * @param  btn:���������ַ
  * @param  event:�¼�����
  * @retval ��
  */
static void ButtonPower_EventHandler(ButtonEvent* btn, int event)
{
    if(btn == &btPOWER)
    {
        /*�����ػ�*/
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            Power_Shutdown();
        }
    }
}

/**
  * @brief  ������ʼ��
  * @param  ��
  * @retval ��
  */
void Button_Init()
{
    /*��������*/
    pinMode(KEY_UP_Pin, INPUT_PULLUP);
    pinMode(KEY_OK_Pin, INPUT_PULLUP);
    pinMode(KEY_DOWN_Pin, INPUT_PULLUP);
    
    /*�����¼�*/
    btUP.EventAttach(Button_EventHandler);
    btOK.EventAttach(Button_EventHandler);
    btDOWN.EventAttach(Button_EventHandler);
    btPOWER.EventAttach(ButtonPower_EventHandler);
}

/**
  * @brief  ������ظ���
  * @param  ��
  * @retval ��
  */
void Button_Update()
{
    btUP.EventMonitor(!digitalRead(KEY_UP_Pin));
    btOK.EventMonitor(!digitalRead(KEY_OK_Pin));
    btDOWN.EventMonitor(!digitalRead(KEY_DOWN_Pin));
    btPOWER.EventMonitor(digitalRead(POWER_EN_Pin));
}
