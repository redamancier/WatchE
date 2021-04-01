#include "GUI/DisplayPrivate.h"
#include "Basic/CommonMacro.h"
#include "BSP/BSP.h"

/*��ҳ�洰��*/
static lv_obj_t * appWindow;

/*������*/
static lv_obj_t * labelTitle;

/*�������ָ���*/
static lv_obj_t * lineTitle;

/*ͼ����ʾ���������ڲü���ʾ*/
static lv_obj_t * contDisp;

/*ͼ��������*/
static lv_obj_t * contICON;

/*ͼƬ��Դ*/
LV_IMG_DECLARE(ImgStopWatch);
LV_IMG_DECLARE(ImgMountain);
LV_IMG_DECLARE(ImgLight);
LV_IMG_DECLARE(ImgTimeCfg);
LV_IMG_DECLARE(ImgInfo);
LV_IMG_DECLARE(ImgButterfly);

/*ͼ����Ϣ����*/
typedef struct{
    const void * src_img;   //ͼƬ���ݵ�ַ
    const char * text;      //��������
    lv_obj_t * img;         //ͼƬ�ؼ�
    uint8_t pageID;         //��Ӧ��ҳ��ID
}ICON_TypeDef;

/*ͼ���鶨��*/
static ICON_TypeDef ICON_Grp[] = 
{
    {.src_img = &ImgStopWatch, .text = "StopWatch",  .pageID = PAGE_StopWatch},  //���
    {.src_img = &ImgMountain,  .text = "Altitude",   .pageID = PAGE_Altitude},   //���α�
    {.src_img = &ImgLight,     .text = "Backlight",  .pageID = PAGE_Backlight},  //��������
    {.src_img = &ImgTimeCfg,   .text = "TimeCfg",    .pageID = PAGE_TimeCfg},    //ʱ������
    {.src_img = &ImgButterfly, .text = "Game",       .pageID = PAGE_Game},       //��Ϸ
    {.src_img = &ImgInfo,      .text = "About",      .pageID = PAGE_About}       //����
};

/*ͼ����*/
static const uint8_t ICON_IntervalPixel = 20;
/*ͼ���С*/
static const uint8_t ICON_Size = 50;
/*��ǰѡ�е�ͼ������*/
static int8_t ICON_NowSelIndex = 0;
/*ͼ���������ֵ*/
#define ICON_MAX_INDEX (__Sizeof(ICON_Grp) - 1)

/**
  * @brief  ����ͼ����
  * @param  ��
  * @retval ��
  */
static void ICON_Grp_Create()
{
    contDisp = lv_cont_create(appWindow, NULL);
    lv_cont_set_style(contDisp, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_set_size(contDisp, ICON_Size + 20, APP_WIN_HEIGHT - lv_obj_get_y(lineTitle) - 20);
    lv_obj_align(contDisp, lineTitle, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    
    contICON = lv_cont_create(contDisp, NULL);
    lv_cont_set_style(contICON, LV_CONT_STYLE_MAIN, &lv_style_transp);
    lv_obj_set_size(contICON, lv_obj_get_width(contDisp), (ICON_Size + ICON_IntervalPixel) * __Sizeof(ICON_Grp));
    lv_obj_set_y(contICON, lv_obj_get_height(contDisp));
    
    
//    lv_obj_t * contSel = lv_cont_create(contDisp, NULL);
//    lv_obj_set_size(contSel, ICON_Size + 20, ICON_Size + 20);
//    lv_obj_align(contSel, contDisp, LV_ALIGN_CENTER, 0, 0);
//    static lv_style_t style_cont;
//    style_cont = *lv_cont_get_style(contSel, LV_CONT_STYLE_MAIN);
//    style_cont.body.opa = LV_OPA_0;
//    style_cont.body.border.opa = LV_OPA_COVER;
//    style_cont.body.border.width = 2;
//    style_cont.body.border.color = LV_COLOR_RED;
//    style_cont.body.radius = 5;
//    lv_cont_set_style(contSel, LV_CONT_STYLE_MAIN, &style_cont);
    
    /*����ͼ����*/
    for(int i = 0; i < __Sizeof(ICON_Grp); i++)
    {
        /*����ͼƬ�ؼ�*/
        lv_obj_t * img = lv_img_create(contICON, NULL);
        ICON_Grp[i].img = img;
        lv_img_set_src(img, ICON_Grp[i].src_img);
        lv_obj_align(img, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
        
        /*����ƫ����*/
        int y_offset = (ICON_Size - lv_obj_get_height(img)) / 2;
        
        lv_obj_set_y(img, (ICON_Size + ICON_IntervalPixel) * i + y_offset);
    }
}

/**
  * @brief  ����������
  * @param  ��
  * @retval ��
  */
static void Title_Create()
{
    LV_FONT_DECLARE(HandGotn_20);
    labelTitle = lv_label_create(appWindow, NULL);
    
    static lv_style_t style_label;
    style_label = *lv_label_get_style(labelTitle, LV_LABEL_STYLE_MAIN);
    style_label.text.font = &HandGotn_20;
    style_label.text.color = LV_COLOR_WHITE;
    lv_label_set_style(labelTitle, LV_LABEL_STYLE_MAIN, &style_label);
    
    /*Ĭ��ѡ�е��ǵڶ���ͼ��*/
    lv_label_set_static_text(labelTitle, ICON_Grp[1].text);
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
  * @brief  �ƶ���ѡ��ͼ��
  * @param  iconIndex:Ŀ��ͼ�������
  * @retval ��
  */
static void ICON_Grp_MoveFocus(uint8_t iconIndex)
{
    if(iconIndex > ICON_MAX_INDEX)
        return;
    
    /*�ı������������˵��*/
    lv_label_set_static_text(labelTitle, ICON_Grp[iconIndex].text);
    
    /*����Ŀ��Y����*/
    int16_t target_y = -(ICON_Size + ICON_IntervalPixel) * (iconIndex - 1);

    /*ִ�л�������*/
    LV_OBJ_ADD_ANIM(contICON, y, target_y, LV_ANIM_TIME_DEFAULT);
}

/**
  * @brief  �����ƶ�ѡ�е�ͼ��
  * @param  dir:����
  * @retval ��
  */
static void ICON_Grp_Move(int8_t dir)
{
    /*���޶���Χ���ƶ�*/
    __ValuePlus(ICON_NowSelIndex, dir, 0, ICON_MAX_INDEX);
    
    /*�ƶ�����ͼ��*/
    ICON_Grp_MoveFocus(ICON_NowSelIndex);
}

/**
  * @brief  ����������Ӱ
  * @param  ��
  * @retval ��
  */
static void ImgShadow_Create()
{
    LV_IMG_DECLARE(ImgShadowUp);
    LV_IMG_DECLARE(ImgShadowDown);
    
    lv_obj_t * imgUp = lv_img_create(contDisp, NULL);
    lv_img_set_src(imgUp, &ImgShadowUp);
    lv_obj_align(imgUp, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    
    lv_obj_t * imgDown = lv_img_create(contDisp, NULL);
    lv_img_set_src(imgDown, &ImgShadowDown);
    lv_obj_align(imgDown, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
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
    Title_Create();
    ICON_Grp_Create();
    ImgShadow_Create();
    
    /*ͼ�껬����һ��ѡ�е�ͼ��*/
    ICON_Grp_MoveFocus(ICON_NowSelIndex);
}

/**
  * @brief  ҳ���˳��¼�
  * @param  ��
  * @retval ��
  */
static void Exit()
{
    /*ͼ��ȫ������*/
    LV_OBJ_ADD_ANIM(contICON, y, lv_obj_get_height(contDisp) + ICON_Size, LV_ANIM_TIME_DEFAULT);
    PageDelay(LV_ANIM_TIME_DEFAULT);
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
            /*����OK���˳���һ��ҳ��*/
            page.PagePop();
        }
        else if(event == ButtonEvent::EVENT_ButtonClick)
        {
            /*����OK�������Ӧҳ��*/
            uint8_t pageID = ICON_Grp[ICON_NowSelIndex].pageID;
            if(pageID != PAGE_NONE)
            {
                page.PagePush(pageID);
            }
        }
    }
    
    if(event == ButtonEvent::EVENT_ButtonPress || event == ButtonEvent::EVENT_ButtonLongPressRepeat)
    {
        /*���»򳤰����¼���ͼ������ѡ��*/
        if(btn == &btUP)
        {
            ICON_Grp_Move(-1);
        }
        if(btn == &btDOWN)
        {
            ICON_Grp_Move(+1);
        }
    }
}

/**
  * @brief  ҳ��ע��
  * @param  pageID:Ϊ��ҳ������ID��
  * @retval ��
  */
void PageRegister_MainMenu(uint8_t pageID)
{
    /*��ȡ�������ҳ��Ĵ���*/
    appWindow = AppWindow_GetCont(pageID);
    
    /*ע����ҳ�������*/
    page.PageRegister(pageID, Setup, NULL, Exit, Event);
}
