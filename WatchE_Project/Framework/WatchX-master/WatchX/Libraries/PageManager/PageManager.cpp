/*
 * MIT License
 * Copyright (c) 2018-2020 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "PageManager.h"

#define IS_PAGE(page)   ((page)<(MaxPage))

#ifndef NULL
#   define NULL 0
#endif

/**
  * @brief  ��ʼ��ҳ�������
  * @param  pageMax: ҳ���������
  * @param  eventMax: �¼��������
  * @retval ��
  */
PageManager::PageManager(uint8_t pageMax, uint8_t pageStackSize)
{
    MaxPage = pageMax;
    NewPage = NULL;
    OldPage = NULL;
    IsPageBusy = false;

    /* �����ڴ棬����б� */
    PageList = new PageList_TypeDef[MaxPage];
    for(uint8_t page = 0; page < MaxPage; page++)
    {
        PageClear(page);
    }
    
    /*ҳ��ջ����*/
    PageStackSize = pageStackSize;
    PageStack = new uint8_t[PageStackSize];
    PageStackClear();
}

/**
  * @brief  ҳ�����������
  * @param  ��
  * @retval ��
  */
PageManager::~PageManager()
{
    delete[] PageList;
    delete[] PageStack;
}

/**
  * @brief  ���һ��ҳ��
  * @param  pageID: ҳ����
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::PageClear(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = NULL;
    PageList[pageID].LoopCallback = NULL;
    PageList[pageID].ExitCallback = NULL;
    PageList[pageID].EventCallback = NULL;

    return true;
}

/**
  * @brief  ע��һ������ҳ�棬����һ����ʼ��������ѭ���������˳��������¼�����
  * @param  pageID: ҳ����
  * @param  setupCallback: ��ʼ�������ص�
  * @param  loopCallback: ѭ�������ص�
  * @param  exitCallback: �˳������ص�
  * @param  eventCallback: �¼������ص�
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::PageRegister(
    uint8_t pageID,
    CallbackFunction_t setupCallback,
    CallbackFunction_t loopCallback,
    CallbackFunction_t exitCallback,
    EventFunction_t eventCallback
)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = setupCallback;
    PageList[pageID].LoopCallback = loopCallback;
    PageList[pageID].ExitCallback = exitCallback;
    PageList[pageID].EventCallback = eventCallback;
    return true;
}

/**
  * @brief  ҳ���¼�����
  * @param  obj: �����¼��Ķ���
  * @param  event: �¼����
  * @retval ��
  */
void PageManager::PageEventTransmit(void* obj, int event)
{
    /*���¼����ݵ���ǰҳ��*/
    if(PageList[NowPage].EventCallback != NULL)
        PageList[NowPage].EventCallback(obj, event);
}

/**
  * @brief  �л���ָ��ҳ��
  * @param  pageID: ҳ����
  * @retval ��
  */
void PageManager::PageChangeTo(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return;
    
    /*���ҳ���Ƿ�æµ*/
    if(!IsPageBusy)
    {
        /*��ҳ��ID*/
        NextPage = NewPage = pageID;

        /*���Ϊæµ״̬*/
        IsPageBusy = true;
    }
}

/**
  * @brief  ҳ��ѹջ����ת����ҳ��
  * @param  pageID: ҳ����
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::PagePush(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;
    
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return false; 
    
    /*��ֹջ���*/
    if(PageStackTop >= PageStackSize - 1)
        return false;
    
    /*��ֹ�ظ�ҳ��ѹջ*/
    if(pageID == PageStack[PageStackTop])
        return false;

    /*ջ��ָ������*/
    PageStackTop++;
    
    /*ҳ��ѹջ*/
    PageStack[PageStackTop] = pageID;
    
    /*ҳ����ת*/
    PageChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  ҳ�浯ջ����ת����һ��ҳ��
  * @param  ��
  * @retval true:�ɹ� false:ʧ��
  */
bool PageManager::PagePop()
{
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return false; 
    
    /*��ֹջ���*/
    if(PageStackTop == 0)
        return false;
    
    /*��յ�ǰҳ��*/
    PageStack[PageStackTop] = 0;
    
    /*��ջ��ջ��ָ������*/
    PageStackTop--;
    
    /*ҳ����ת*/
    PageChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  ���ҳ��ջ
  * @param  ��
  * @retval ��
  */
void PageManager::PageStackClear()
{
    /*���ҳ���Ƿ�æµ*/
    if(IsPageBusy)
       return; 
    
    /*���ջ����������*/
    for(uint8_t i = 0; i < PageStackSize; i++)
    {
        PageStack[i] = 0;
    }
    /*ջ��ָ�븴λ*/
    PageStackTop = 0;
}

/**
  * @brief  ҳ�������״̬��
  * @param  ��
  * @retval ��
  */
void PageManager::Running()
{
    /*ҳ���л��¼�*/
    if(NewPage != OldPage)
    {
        /*���Ϊæµ״̬*/
        IsPageBusy = true;

        /*������ҳ���˳��¼�*/
        if(PageList[OldPage].ExitCallback != NULL && IS_PAGE(OldPage))
            PageList[OldPage].ExitCallback();
        
        /*��Ǿ�ҳ��*/
        LastPage = OldPage;
        
        /*�����ҳ��Ϊ��ǰҳ��*/
        NowPage = NewPage;

        /*������ҳ���ʼ���¼�*/
        if(PageList[NewPage].SetupCallback != NULL && IS_PAGE(NewPage))
            PageList[NewPage].SetupCallback();

        /*��ҳ���ʼ����ɣ����Ϊ��ҳ��*/
        OldPage = NewPage;
    }
    else
    {
        /*���ҳ�治æµ������ѭ��״̬*/
        IsPageBusy = false;
        
        /*ҳ��ѭ���¼�*/
        if(PageList[NowPage].LoopCallback != NULL && IS_PAGE(NowPage))
            PageList[NowPage].LoopCallback();
    }
}
