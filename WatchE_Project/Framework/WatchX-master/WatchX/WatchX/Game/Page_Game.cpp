#include "GUI/DisplayPrivate.h"
#include "GamePrivate.h"
#include "BSP/BSP.h"

static lv_obj_t * appWindow;
static lv_obj_t * contGameDisp;
static lv_obj_t * ledGrp[GAME_LED_MAX];
static lv_task_t * taskGame;

static void Task_GameScreenUpdate(lv_task_t * task)
{
    Game_SetButtonState(GAME_BUTTON_UP, !digitalRead(KEY_UP_Pin));
    Game_SetButtonState(GAME_BUTTON_DOWN, !digitalRead(KEY_DOWN_Pin));
    Game_SetButtonState(GAME_BUTTON_RIGHT, !digitalRead(KEY_UP_Pin));
    Game_SetButtonState(GAME_BUTTON_LEFT, !digitalRead(KEY_DOWN_Pin));
    Game_SetButtonState(GAME_BUTTON_A, !digitalRead(KEY_OK_Pin));
    Game_SetButtonState(GAME_BUTTON_B, !digitalRead(KEY_OK_Pin));
    Game_DispTaskUpdate();
}

static void ContGameDisp_Create()
{
    contGameDisp = lv_cont_create(appWindow, NULL);
    lv_obj_set_size(contGameDisp, 128 + 4, 64 + 4);
    lv_obj_align(contGameDisp, NULL, LV_ALIGN_CENTER, 0, 0);
}

void Game_DispGetContPos(int16_t* x, int16_t* y)
{
    *x = lv_obj_get_x(contGameDisp) + 2;
    *y = lv_obj_get_y(contGameDisp) + 2;
}

static void LED_Create()
{
    /*Create a style for the LED*/
    static lv_style_t style_led[GAME_LED_MAX];
    lv_style_copy(&style_led[GAME_LED_RED], &lv_style_pretty_color);
    lv_style_copy(&style_led[GAME_LED_GREEN], &lv_style_pretty_color);
    lv_style_copy(&style_led[GAME_LED_BLUE], &lv_style_pretty_color);

    ledGrp[GAME_LED_RED]  = lv_led_create(appWindow, NULL);
    lv_led_set_style(ledGrp[GAME_LED_RED], LV_LED_STYLE_MAIN, &style_led[GAME_LED_RED]);
    lv_obj_set_color(ledGrp[GAME_LED_RED], LV_COLOR_RED);
    lv_obj_align(ledGrp[GAME_LED_RED], contGameDisp, LV_ALIGN_OUT_BOTTOM_MID, -20, 5);
    lv_led_off(ledGrp[GAME_LED_RED]);

    ledGrp[GAME_LED_GREEN]  = lv_led_create(appWindow, NULL);
    lv_led_set_style(ledGrp[GAME_LED_GREEN], LV_LED_STYLE_MAIN, &style_led[GAME_LED_GREEN]);
    lv_obj_set_color(ledGrp[GAME_LED_GREEN], LV_COLOR_GREEN);
    lv_obj_align(ledGrp[GAME_LED_GREEN], contGameDisp, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
    lv_led_off(ledGrp[GAME_LED_GREEN]);

    ledGrp[GAME_LED_BLUE] = lv_led_create(appWindow, NULL);
    lv_led_set_style(ledGrp[GAME_LED_BLUE], LV_LED_STYLE_MAIN, &style_led[GAME_LED_BLUE]);
    lv_obj_set_color(ledGrp[GAME_LED_BLUE], LV_COLOR_BLUE);
    lv_obj_align(ledGrp[GAME_LED_BLUE], contGameDisp, LV_ALIGN_OUT_BOTTOM_MID, 20, 5);
    lv_led_off(ledGrp[GAME_LED_BLUE]);
}

void Game_SetLEDState(uint8_t led_id, bool val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id])
    {
        val ? lv_led_on(ledGrp[led_id]) : lv_led_off(ledGrp[led_id]);
    }
}

void Game_SetLEDBright(uint8_t led_id, uint8_t val)
{
    if(led_id < GAME_LED_MAX && ledGrp[led_id])
    {
        lv_led_set_bright(ledGrp[led_id], val);
    }
}

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
    lv_obj_move_foreground(appWindow);
    
    lv_obj_set_color(appWindow, LV_COLOR_BLACK);
    
    Game_Begin();
    
    ContGameDisp_Create();
    LED_Create();
    
    int16_t x,y;
    Game_DispGetContPos(&x, &y);
    Game_DispSetBasePoint(x, y);
    
    taskGame = lv_task_create(Task_GameScreenUpdate, 20, LV_TASK_PRIO_HIGH, 0);
}

/**
  * @brief  ҳ��ѭ���¼�
  * @param  ��
  * @retval ��
  */
static void Loop()
{
    GamePage.Running();
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    GamePage.PageChangeTo(GAME_NONE);
    GamePage.Running();
    GamePage.Running();
    lv_task_del(taskGame);
    lv_obj_clean(appWindow);
}

/**
  * @brief  ҳ���¼�
  * @param  event:�¼����
  * @param  param:�¼�����
  * @retval ��
  */
static void Event(void* param, int event)
{
    if(event == ButtonEvent::EVENT_ButtonLongPressed)
    {
        if(param == &btOK)
        {
            page.PagePop();
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_Game(uint8_t pageID)
{
    appWindow = AppWindow_GetCont(pageID);
    page.PageRegister(pageID, Setup, Loop, Exit, Event);
}
