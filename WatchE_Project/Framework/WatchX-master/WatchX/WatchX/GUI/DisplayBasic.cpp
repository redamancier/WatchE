#include "GUI/DisplayPrivate.h"
#include "BSP/BSP.h"

/*ʵ������Ļ����*/
SCREEN_CLASS screen(TFT_CS_Pin, TFT_DC_Pin, TFT_RST_Pin);

/*ʵ����ҳ�������*/
PageManager page(PAGE_MAX);

/*ҳ��ע����*/
#define PAGE_REG(name)\
do{\
    extern void PageRegister_##name(uint8_t pageID);\
    PageRegister_##name(PAGE_##name);\
}while(0)

/**
  * @brief  ҳ���ʼ��
  * @param  ��
  * @retval ��
  */
static void Pages_Init()
{
    PAGE_REG(DialPlate);    //����
    PAGE_REG(MainMenu);     //���˵�
    PAGE_REG(TimeCfg);      //ʱ������
    PAGE_REG(Backlight);    //��������
    PAGE_REG(StopWatch);    //���
    PAGE_REG(Altitude);     //���α�
    PAGE_REG(About);        //����
    PAGE_REG(Game);         //��Ϸ
    
    page.PagePush(PAGE_DialPlate);//�򿪱���
}

/**
  * @brief  ��ʾ��ʼ��
  * @param  ��
  * @retval ��
  */
void Display_Init()
{
    /*����ر�*/
    Backlight_SetValue(0);
    
    /*��Ļ��ʼ��*/
    screen.begin();
    screen.setRotation(0);
    screen.fillScreen(screen.Black);
    
    /*�Զ������ʼ��*/
    DisplayError_Init();
    
    /*lvgl��ʼ��*/
    lv_init();
    lv_port_disp_init();
    lv_theme_set_current(lv_theme_night_init(200, NULL));
    
    /*APP���ڳ�ʼ��*/
    AppWindow_Create();
    
    /*ҳ���ʼ��*/
    Pages_Init();
    
    /*���⽥��*/
    Backlight_SetGradual(Backlight_GetBKP(), 1000);
}

/**
  * @brief  ��ʾ����
  * @param  ��
  * @retval ��
  */
void Display_Update()
{
    lv_task_handler();
    page.Running();
}

/**
  * @brief  ҳ��������ʱ������lvgl����
  * @param  ��
  * @retval ��
  */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = millis();
    while(millis() - lastTime <= ms)
    {
        lv_task_handler();
    }
}

//static void Display_FPSTest()
//{
//    float Ftime, Ltime;
//    float FPS;
//    Ftime = millis();
//    int i;
//    for(i = 100; i > 0; i--)
//    {
//        screen.fillScreen(screen.Black);
//        screen.fillScreen(screen.White);
//    }
//    Ltime = millis() - Ftime;
//    FPS = 100.0f / (Ltime / 1000.0f) * 2.0f;
//    screen.fillScreen(screen.Blue);
//    screen.setTextSize(1);
//    screen.setTextColor(screen.White);
//    screen.setCursor(0, 0);
//    screen.print(Ltime);
//    screen.setCursor(45, 0);
//    screen.print("ms");
//    screen.setCursor(0, 9);
//    screen.print("FPS:");
//    screen.setCursor(25, 9);
//    screen.print(FPS);
//    while(1);
//}
