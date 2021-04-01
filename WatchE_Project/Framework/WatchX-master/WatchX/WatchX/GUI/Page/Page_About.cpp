#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP/BSP.h"

static lv_obj_t * appWindow;

static lv_obj_t * labelTitle;
static lv_obj_t * lineTitle;

static lv_obj_t * imgLogo;
static lv_obj_t * imgProg;
static lv_obj_t * imgArt;

static lv_obj_t * labelDesigner;
static lv_obj_t * labelFwInfo;

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

static void Img_Create()
{
    LV_IMG_DECLARE(ImgWatchX);
    LV_IMG_DECLARE(ImgVIFEXTech);
    LV_IMG_DECLARE(ImgTrigger);
    
    imgLogo = lv_img_create(appWindow, NULL);
    lv_img_set_src(imgLogo, &ImgWatchX);
    lv_obj_align(imgLogo, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_opa_scale_enable(imgLogo, true);
    lv_obj_set_opa_scale(imgLogo, LV_OPA_TRANSP);
    
    imgProg = lv_img_create(appWindow, imgLogo);
    lv_img_set_src(imgProg, &ImgVIFEXTech);
    lv_obj_align(imgProg, NULL, LV_ALIGN_CENTER, 0, 20);
    
    imgArt = lv_img_create(appWindow, imgLogo);
    lv_img_set_src(imgArt, &ImgTrigger);
    lv_obj_align(imgArt, imgProg, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

static void LabelInfo_Create()
{
    LV_FONT_DECLARE(HandGotn_14);
    labelDesigner = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelDesigner, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_14;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelDesigner, LV_LABEL_STYLE_MAIN, &style_label);
    lv_label_set_static_text(labelDesigner, "- DEVELOPER -");
    lv_obj_align(labelDesigner, imgProg, LV_ALIGN_OUT_TOP_MID, 0, -5);
    lv_obj_set_opa_scale_enable(labelDesigner, true);
    lv_obj_set_opa_scale(labelDesigner, LV_OPA_TRANSP);
    
    
    labelFwInfo = lv_label_create(appWindow, labelDesigner);
    lv_label_set_style(labelFwInfo, LV_LABEL_STYLE_MAIN, &style_label);
    lv_label_set_static_text(labelFwInfo, WX_NAME" "WX_SOFTWARE_VERSION"\n"__DATE__);
    lv_obj_align(labelFwInfo, imgArt, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
}

static void PageAnimOpen(bool open)
{
    int step = open ? 0 : 4;
    
    int cnt = 5;
    
    lv_opa_t opa_target = open ? LV_OPA_COVER : LV_OPA_TRANSP;

    while(cnt--)
    {
        switch(step)
        {
            case 0:
                LV_OBJ_ADD_ANIM(
                    imgLogo, opa_scale,
                    opa_target,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 1;
                break;
            case 1:
                LV_OBJ_ADD_ANIM(
                    labelDesigner, opa_scale,
                    opa_target,
                    LV_ANIM_TIME_DEFAULT
                );
                step = open ? 2 : 0;
                break;
            case 2:
                LV_OBJ_ADD_ANIM(
                    imgProg, opa_scale,
                    opa_target,
                    LV_ANIM_TIME_DEFAULT
                );
                step = open ? 3 : 1;
                break;
            case 3:
                LV_OBJ_ADD_ANIM(
                    imgArt, opa_scale,
                    opa_target,
                    LV_ANIM_TIME_DEFAULT
                );
                step = open ? 4 : 2;
                break;
             case 4:
                LV_OBJ_ADD_ANIM(
                    labelFwInfo, opa_scale,
                    opa_target,
                    LV_ANIM_TIME_DEFAULT
                );
                step = 3;
                break;
        }
        PageDelay(LV_ANIM_TIME_DEFAULT + 100);
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
    
    Title_Create("About");
    Img_Create();
    LabelInfo_Create();
    LabelInfo_Create();
    PageAnimOpen(true);
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    PageAnimOpen(false);
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
    
    if(event == ButtonEvent::EVENT_ButtonPress || event == ButtonEvent::EVENT_ButtonLongPressRepeat)
    {
        if(btn == &btUP)
        {
        }
        if(btn == &btDOWN)
        {
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_About(uint8_t pageID)
{
    /*��ȡ�������ҳ��Ĵ���*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*ע����ҳ�������*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
