#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP/BSP.h"

/*��ҳ�洰��*/
static lv_obj_t * appWindow;

/*������*/
static lv_obj_t * labelTitle;

/*�������ָ���*/
static lv_obj_t * lineTitle;

/*���߱�*/
static lv_obj_t * lmeterSec;
/*ʱ��ǩ*/
static lv_obj_t * labelHour;
/*�����ǩ*/
static lv_obj_t * labelMinSec;
/*�����ǩ*/
static lv_obj_t * labelMs;

/*��ʷ��¼����*/
static lv_obj_t * contRecord;
/*��¼3����ʷ*/
#define SW_RECORD_MAX 3
/*��ʷ��¼��ǩ*/
static lv_obj_t * labelRecord_Grp[SW_RECORD_MAX];

/**
  * @brief  ����������
  * @param  text:�������ı�
  * @retval ��
  */
static void Title_Create(const char * text)
{
    LV_FONT_DECLARE(HandGotn_20);
    labelTitle = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTitle, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTitle, LV_LABEL_STYLE_MAIN, &style_label);
    
    lv_label_set_static_text(labelTitle, text);
    lv_obj_align(labelTitle, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_obj_set_auto_realign(labelTitle, true);
    
    lineTitle = lv_line_create(appWindow, NULL);
    static lv_point_t line_points[] = { {0, 0}, {APP_WIN_WIDTH, 0}};
    lv_line_set_points(lineTitle, line_points, 2);
    static lv_style_t style_line;
    style_line = *lv_line_get_style(lineTitle, LV_LINE_STYLE_MAIN);
    style_line.line.color = LV_COLOR_RED;
    style_line.line.width = 2;
    style_line.line.rounded = 1;
    lv_line_set_style(lineTitle, LV_LINE_STYLE_MAIN, &style_line);
    lv_obj_align(lineTitle, labelTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
}

/**
  * @brief  �������߱�
  * @param  ��
  * @retval ��
  */
static void LmeterSec_Create()
{
    static lv_style_t style_lmeter;
    style_lmeter = lv_style_pretty_color;
    
    style_lmeter.body.main_color = LV_COLOR_RED;
    style_lmeter.body.grad_color = LV_COLOR_RED;
    style_lmeter.body.padding.left = 10;
    style_lmeter.line.width = 1;
    style_lmeter.line.color = LV_COLOR_SILVER;

    lmeterSec = lv_lmeter_create(appWindow, NULL);
    lv_lmeter_set_range(lmeterSec, 0, 59);
    lv_lmeter_set_angle_offset(lmeterSec, 180);
//    lv_lmeter_set_value(lmeterSec, 10);
    lv_lmeter_set_scale(lmeterSec, 360, 31);
    lv_lmeter_set_style(lmeterSec, LV_LMETER_STYLE_MAIN, &style_lmeter);
    lv_obj_set_size(lmeterSec, 120, 120);
    lv_obj_set_opa_scale_enable(lmeterSec, true);
    lv_obj_set_opa_scale(lmeterSec, LV_OPA_TRANSP);
    lv_obj_align(lmeterSec, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

/**
  * @brief  ����ʱ���ǩ
  * @param  ��
  * @retval ��
  */
static void LabelTime_Create()
{
    LV_FONT_DECLARE(HandGotn_26);
    
    labelMinSec = lv_label_create(lmeterSec, NULL);
    
    static lv_style_t style_label1;
    style_label1 = *lv_label_get_style(labelMinSec, LV_LABEL_STYLE_MAIN);
    style_label1.text.font = &HandGotn_26;
    style_label1.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelMinSec, LV_LABEL_STYLE_MAIN, &style_label1);
    
    lv_label_set_recolor(labelMinSec, true);
    lv_label_set_text(labelMinSec, "00#FF0000 :#00");
    lv_obj_align(labelMinSec, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelMinSec, true);
    
    labelHour = lv_label_create(lmeterSec, NULL);
    
    LV_FONT_DECLARE(HandGotn_20);
    static lv_style_t style_label2;
    style_label2 = *lv_label_get_style(labelHour, LV_LABEL_STYLE_MAIN);
    style_label2.text.font = &HandGotn_20;
    style_label2.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelHour, LV_LABEL_STYLE_MAIN, &style_label2);
    
    lv_label_set_text(labelHour, "00");
    lv_obj_align(labelHour, labelMinSec, LV_ALIGN_OUT_TOP_MID, 0, -3);
    lv_obj_set_auto_realign(labelHour, true);
    
    labelMs = lv_label_create(lmeterSec, NULL);
    lv_label_set_style(labelMs, LV_LABEL_STYLE_MAIN, &style_label2);
    lv_label_set_text(labelMs, ".000");
    lv_obj_align(labelMs, labelMinSec, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
    lv_obj_set_auto_realign(labelMs, true);
}

/**
  * @brief  ������ʷ��¼����
  * @param  ��
  * @retval ��
  */
static void ContRecord_Create()
{
    contRecord = lv_cont_create(appWindow, NULL);
    lv_obj_set_size(contRecord, 128, 60);
//    lv_obj_align(contRecord, lmeterSec, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_y(contRecord, lv_obj_get_height(appWindow) + 10);
    
//    LV_FONT_DECLARE(HandGotn_14);
//    static lv_style_t style_label;
//    style_label = lv_style_plain_color;
//    style_label.body.main_color = LV_COLOR_BLACK;
//    style_label.body.grad_color = LV_COLOR_BLACK;
//    style_label.text.font = &HandGotn_14;
//    style_label.text.color = LV_COLOR_SILVER;
    
//    lv_obj_t * label1 = lv_label_create(appWindow, NULL);
//    lv_label_set_body_draw(label1, true);
//    lv_label_set_static_text(label1, " Record ");
//    lv_obj_align(label1, contRecord, LV_ALIGN_OUT_TOP_MID, 0, 10);
//    lv_obj_set_auto_realign(label1, true);
//    lv_label_set_style(label1, LV_LABEL_STYLE_MAIN, &style_label);
    
    static lv_style_t style_cont;
    style_cont = lv_style_plain;
    style_cont.body.opa = LV_OPA_0;
    style_cont.body.border.opa = LV_OPA_COVER;
    style_cont.body.border.width = 1;
    style_cont.body.border.color = LV_COLOR_RED;
    style_cont.body.radius = 10;
    lv_cont_set_style(contRecord, LV_CONT_STYLE_MAIN, &style_cont);
    lv_obj_set_opa_scale_enable(contRecord, true);
    lv_obj_set_opa_scale(contRecord, LV_OPA_TRANSP);
}

/**
  * @brief  ������ʷ��¼��ǩ
  * @param  ��
  * @retval ��
  */
static void LabelRecord_Create()
{
    LV_FONT_DECLARE(HandGotn_14);
    static lv_style_t style_label;
    style_label = lv_style_plain_color;
    style_label.body.main_color = LV_COLOR_BLACK;
    style_label.body.grad_color = LV_COLOR_BLACK;
    style_label.text.font = &HandGotn_14;
    style_label.text.color = LV_COLOR_WHITE;
    const lv_coord_t y_mod[SW_RECORD_MAX] = {-15, 0, 15};
    
    for(int i = 0; i < SW_RECORD_MAX; i++)
    {
        lv_obj_t * label = lv_label_create(contRecord, NULL);
        lv_label_set_text(label, "--:--:--.---");
        lv_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 15, y_mod[i] + 2);
        lv_obj_set_auto_realign(label, true);
        lv_label_set_style(label, LV_LABEL_STYLE_MAIN, &style_label);
        labelRecord_Grp[i] = label;
    }
}

/**
  * @brief  ҳ�涯������
  * @param  open:�Ƿ�Ϊҳ�����ʱ�Ķ���
  * @retval ��
  */
static void PagePlayAnim(bool open)
{
    /*����0~1����������*/
    /*�˳�1~0����������*/
    int step = open ? 0 : 1;
                          
    /*����������*/
    int cnt = 2;
    while(cnt--)
    {
        /*����״̬��*/
        switch(step)
        {
            case 0:
                /*��ʷ��¼������Y���ƶ�����*/
                LV_OBJ_ADD_ANIM(
                    contRecord, y, 
                    open ? lv_obj_get_y(lmeterSec) + lv_obj_get_height(lmeterSec) + 20 : lv_obj_get_height(appWindow) + 20,
                    LV_ANIM_TIME_DEFAULT
                );
                /*��ʷ��¼������͸���ȱ仯����*/
                LV_OBJ_ADD_ANIM(
                    contRecord, opa_scale,
                    open ? LV_OPA_COVER : LV_OPA_TRANSP,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 1;
                break;
            case 1:
                /*���߱�͸���ȱ仯����*/
                LV_OBJ_ADD_ANIM(
                    lmeterSec, opa_scale,
                    open ? LV_OPA_COVER : LV_OPA_TRANSP,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 0;
                break;
        }
        /*ҳ����ʱ���ȴ������������*/
        PageDelay(LV_ANIM_TIME_DEFAULT);
    }
}

/*���״̬*/
typedef enum{
    STATE_READY,     //����
    STATE_RUNNING,   //������
    STATE_PAUSE,     //��ͣ
}SW_State_Type;

/*���״̬��������*/
static const char* SW_StateStr[] = {
    "Ready",
    "Running",
    "Pause",
};

/*���״̬*/
static SW_State_Type SW_State = STATE_READY;

/*���������*/
static uint32_t SW_TickMs = 0;

/*��ʷ��¼����*/
static uint8_t  SW_RecordIndex = 0;

/*��ʷ��¼*/
static uint32_t  SW_RecordMs[SW_RECORD_MAX];

/*����ǩˢ��������*/
static lv_task_t * taskLabelUpdate;

/**
  * @brief  ����ǩˢ������
  * @param  task:������
  * @retval ��
  */
static void SW_LabelUpdate(lv_task_t * task)
{
    uint16_t hh = (SW_TickMs / (3600 * 1000)) % 100;
    uint16_t mm = (SW_TickMs / (60 * 1000)) % 60;
    uint16_t ss = (SW_TickMs / 1000) % 60;
    uint16_t ms = SW_TickMs % 1000;
    
    static uint8_t cnt;
    if(cnt++ > 20)
    {
        lv_lmeter_set_value(lmeterSec, ss);
        lv_label_set_text_fmt(labelHour, "%02d", hh);
        lv_label_set_text_fmt(labelMinSec, "%02d#FF0000 :#%02d", mm , ss);
        cnt = 0;
    }
    lv_label_set_text_fmt(labelMs, ".%03d", ms);
}

/**
  * @brief  �����ʷ��¼����
  * @param  ��
  * @retval ��
  */
static void SW_RecordUpdate()
{
    for(int i = 0; i < SW_RECORD_MAX; i++)
    {
        uint32_t ms = SW_RecordMs[i];
        if(ms > 0)
        {
            uint16_t hh = (ms / (3600 * 1000)) % 100;
            uint16_t mm = (ms / (60 * 1000)) % 60;
            uint16_t ss = (ms / 1000) % 60;
            ms = ms % 1000;
            lv_label_set_text_fmt(labelRecord_Grp[i], "%02d:%02d:%02d.%03d", hh, mm, ss, ms);
        }
        else
        {
            lv_label_set_text(labelRecord_Grp[i], "--:--:--.---");
        }
    }
}

/**
  * @brief  �������жϻص�����Ӳ����ʱ������
  * @param  ��
  * @retval ��
  */
static void SW_TimerCallback()
{
    SW_TickMs++;
}

/**
  * @brief  ����ʼ��
  * @param  ��
  * @retval ��
  */
static void SW_Init()
{
    if(SW_State == STATE_READY)
    {
        Timer_SetInterrupt(STOPWATCH_TIM, 2000, SW_TimerCallback);
    }
    else
    {
        lv_label_set_static_text(labelTitle, SW_StateStr[SW_State]);
    }
    /*ˢ����ʷ��¼*/
    SW_RecordUpdate();
    /*ע������*/
    taskLabelUpdate = lv_task_create(SW_LabelUpdate, 11, LV_TASK_PRIO_HIGH, NULL);
    SW_LabelUpdate(taskLabelUpdate);
}

/**
  * @brief  �����ͣ����
  * @param  ��
  * @retval ��
  */
static void SW_StartStop()
{
    if(SW_State == STATE_READY || SW_State == STATE_PAUSE)
    {
        TIM_Cmd(STOPWATCH_TIM, ENABLE);
        SW_State = STATE_RUNNING;
    }
    else if(SW_State == STATE_RUNNING)
    {
        TIM_Cmd(STOPWATCH_TIM, DISABLE);
        SW_State = STATE_PAUSE;
    }
    
    lv_label_set_static_text(labelTitle, SW_StateStr[SW_State]);
}

/**
  * @brief  ���λ
  * @param  ��
  * @retval ��
  */
static void SW_Reset()
{
    if(SW_State == STATE_PAUSE)
    {
        SW_TickMs = 0;
        SW_RecordIndex = 0;
        for(int i = 0; i < SW_RECORD_MAX; i++)
        {
            SW_RecordMs[i] = 0;
        }
        SW_RecordUpdate();
        SW_State = STATE_READY;
        lv_label_set_static_text(labelTitle, SW_StateStr[SW_State]);
    }
}

/**
  * @brief  ����¼
  * @param  ��
  * @retval ��
  */
static void SW_Record()
{
    if(SW_State == STATE_RUNNING)
    {
        if(SW_RecordIndex < SW_RECORD_MAX)
        {
            SW_RecordMs[SW_RecordIndex] = SW_TickMs;
            SW_RecordUpdate();
            SW_RecordIndex++;
        }
    }
}

/**
  * @brief  ҳ���ʼ���¼�
  * @param  ��
  * @retval ��
  */
static void Setup()
{
    /*����ҳ���Ƶ�ǰ̨*/
    lv_obj_move_foreground(appWindow);
    
    Title_Create("StopWatch");
    LmeterSec_Create();
    LabelTime_Create();
    ContRecord_Create();
    LabelRecord_Create();
    SW_Init();
    PagePlayAnim(true);
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    PagePlayAnim(false);
    lv_task_del(taskLabelUpdate);
    lv_obj_clean(appWindow);
}

/**
  * @brief  ҳ���¼�
  * @param  btn:�����¼��İ���
  * @param  event:�¼����
  * @retval ��
  */
static void Event(void* btn, int event)
{
    if(btn == &btOK)
    {
        if(event == ButtonEvent::EVENT_ButtonLongPressed)
        {
            page.PagePop();
        }
    }
    
    if(event == ButtonEvent::EVENT_ButtonPress)
    {
        if(btn == &btUP)
        {
            SW_StartStop();
        }
        if(btn == &btDOWN)
        {
            SW_Record();
        }
    }
    
    if(event == ButtonEvent::EVENT_ButtonLongPressed)
    {
        if(btn == &btDOWN)
        {
            SW_Reset();
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_StopWatch(uint8_t pageID)
{
    /*��ȡ�������ҳ��Ĵ���*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*ע����ҳ�������*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
