#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP/BSP.h"

/*��ҳ�洰��*/
static lv_obj_t * appWindow;

/*������*/
static lv_obj_t * labelTitle;

/*�������ָ���*/
static lv_obj_t * lineTitle;

/*����ֵ��ǩ*/
static lv_obj_t * labelBright;

/*����ָʾ��*/
static lv_obj_t * arcBright;

/*�Զ��ػ�ʱ��*/
static lv_obj_t * labelTime;

/*��С��������*/
static const uint16_t BrightMinVal = 10;

/*����ָʾ���Ƕ�*/
static int16_t ArcNowAngle = 0;

/**
  * @brief  ����ָʾ�������ص�
  * @param  obj:����
  * @param  angle:�Ƕ�0~359
  * @retval ��
  */
static void ArcBright_AnimCallback(lv_obj_t * obj, int16_t angle)
{
    ArcNowAngle = angle;
    if(angle < 180)
    {
        lv_arc_set_angles(obj, 180 - angle ,180);
    }
    else 
    {
        lv_arc_set_angles(obj, 540 - angle ,180);
    }
}

/**
  * @brief  ����ָʾ���趨�Ƕȣ�������ʽ����
  * @param  targetAngle:�Ƕ�
  * @param  anim_time:����ʱ��(Ĭ��500ms)
  * @retval ��
  */
static void BrightArcSetVal(int targetAngle, uint16_t anim_time = 500)
{
    static lv_anim_t a;
    lv_obj_add_anim(
        arcBright, &a,
        (lv_anim_exec_xcb_t)ArcBright_AnimCallback,
        ArcNowAngle,
        targetAngle,
        anim_time
    );
}

/**
  * @brief  �����趨
  * @param  dir:�����򽵵�
  * @retval ��
  */
static void BrightCtrl(int8_t dir)
{
    int16_t brightVal = Backlight_GetValue();
    brightVal += dir;
    __LimitValue(brightVal, BrightMinVal, 1000);
    Backlight_SetValue(brightVal);
    lv_label_set_text_fmt(labelBright, "%d", map(brightVal, BrightMinVal, 1000, 0, 100));
    
    if(dir != 0)
    {
        int targetAngle = map(brightVal, BrightMinVal, 1000, 0, 359);
        BrightArcSetVal(targetAngle);
    }
}

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
  * @brief  ��������ָʾ����
  * @param  ��
  * @retval ��
  */
static void Bright_Create()
{
    static lv_style_t style_arc;
    style_arc = lv_style_plain;
    style_arc.line.color = LV_COLOR_RED;
    style_arc.line.width = 8;
    style_arc.body.shadow.type = LV_SHADOW_FULL;
    style_arc.body.shadow.color = LV_COLOR_RED;
    style_arc.body.shadow.width = 10;

    arcBright = lv_arc_create(appWindow, NULL);
    lv_obj_set_size(arcBright, APP_WIN_WIDTH - 20, APP_WIN_WIDTH - 20);
    lv_arc_set_angles(arcBright, 180, 180);
    lv_arc_set_style(arcBright, LV_ARC_STYLE_MAIN, &style_arc);
    lv_obj_align(arcBright, NULL, LV_ALIGN_CENTER, 0, lv_obj_get_y(lineTitle));
    
    LV_FONT_DECLARE(Morganite_100);
    
    labelBright = lv_label_create(appWindow, NULL);
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelBright, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &Morganite_100;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelBright, LV_LABEL_STYLE_MAIN, &style_label);
    lv_obj_align(labelBright, arcBright, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_auto_realign(labelBright, true);
    lv_obj_set_opa_scale_enable(labelBright, true);
    lv_obj_set_opa_scale(labelBright, LV_OPA_TRANSP);
}

/**
  * @brief  �����Զ��ػ�ʱ���ǩ
  * @param  ��
  * @retval ��
  */
static void LabelTime_Create()
{
    LV_FONT_DECLARE(HandGotn_20);
    
    labelTime = lv_label_create(appWindow, NULL);
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTime, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTime, LV_LABEL_STYLE_MAIN, &style_label);
    lv_obj_align(labelTime, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);
    lv_obj_set_auto_realign(labelTime, true);
    lv_obj_set_hidden(labelTime, true);
    
    lv_label_set_text_fmt(labelTime, "%d Sec", Power_GetAutoLowPowerTimeout());
}

/**
  * @brief  �����Զ��ػ�ʱ��
  * @param  dir:���ӻ����ʱ��
  * @retval ��
  */
static void LabelTime_AddIndex(int8_t dir)
{
    static const uint16_t TimeGrp[] = {
        5, 10, 30, 60, 120, 300
    };
    
    uint16_t time = Power_GetAutoLowPowerTimeout();
    
    int16_t index = 2;
    
    for(int i = 0; i < __Sizeof(TimeGrp); i++)
    {
        if(time == TimeGrp[i])
        {
            index = i;
            break;
        }
    }
    
    __ValuePlus(index, dir, 0, __Sizeof(TimeGrp) - 1);
    Power_SetAutoLowPowerTimeout(TimeGrp[index]);
    lv_label_set_text_fmt(labelTime, "%d Sec", Power_GetAutoLowPowerTimeout());
}

/**
  * @brief  ҳ�涯������
  * @param  open:�Ƿ�Ϊҳ�����ʱ�Ķ���
  * @retval ��
  */
static void PagePlayAnim(bool open)
{
    int step = open ? 0 : 1;
    
    int cnt = 2;
    int targetAngle = 0;
    while(cnt--)
    {
        switch(step)
        {
            case 0:
                targetAngle = open ? map(Backlight_GetValue(), BrightMinVal, 1000, 0, 359) : 0;
                BrightArcSetVal(targetAngle, 400);
                step = 1;
                break;
            case 1:
                LV_OBJ_ADD_ANIM(labelBright, opa_scale, open ? LV_OPA_COVER : LV_OPA_TRANSP, 400);
                step = 0;
                break;
        }
        PageDelay(400);
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
    
    ArcNowAngle = 0;
    
    Title_Create("Backlight");
    Bright_Create();
    LabelTime_Create();

    BrightCtrl(0);
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
        else if(event == ButtonEvent::EVENT_ButtonClick)
        {
            lv_obj_set_hidden(labelTime, !lv_obj_get_hidden(labelTime));
        }
    }
    
    if(event == ButtonEvent::EVENT_ButtonPress || event == ButtonEvent::EVENT_ButtonLongPressRepeat)
    {
        int valPlus = event == ButtonEvent::EVENT_ButtonPress ? 10 : 50;
        if(btn == &btUP)
        {
            /*���ʱ�����أ��͵�������*/
            if(!lv_obj_get_hidden(labelTime))
            {
                LabelTime_AddIndex(+1);
            }
            else
            {
                BrightCtrl(+valPlus);
            }
        }
        if(btn == &btDOWN)
        {
            if(!lv_obj_get_hidden(labelTime))
            {
                LabelTime_AddIndex(-1);
            }
            else
            {
                BrightCtrl(-valPlus);
            }
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_Backlight(uint8_t pageID)
{
    /*��ȡ�������ҳ��Ĵ���*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*ע����ҳ�������*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
