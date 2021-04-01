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
 
/**
  ******************************************************************************
  * @file    PageManager.h
  * @author  _VIFEXTech
  * @version v1.4
  * @date    2020-7-28
  * @brief   ����״̬�����첽ҳ����������������κ�GUI��ܣ�֧�ֲ���ϵͳ
  * @finished: 2018.10.6  - v1.0 ӵ�м򵥵�ҳ���л������¼����ݵ���
  * @upgrade:  2019.4.1   - v1.1 ����ע�ͣ���д�¼����ݵ��ȣ������ظ�����
  * @upgrade:  2019.10.5  - v1.2 ʹ��ջ�ṹ����ҳ��Ƕ�ף�Ĭ��ջ���10��(�ɵ�)
  * @upgrade:  2019.12.4  - v1.3 �޸���Setup��Exit���������ҳ���л���BUG���������
  * @upgrade:  2020.7.28  - v1.4 �޸�EventFunction_t�β�˳���ж�ָ��ʹ�� "ptr != NULL" ����ʽ�����MIT��ԴЭ��
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H

#include <stdint.h>

class PageManager {
    typedef void(*CallbackFunction_t)(void);
    typedef void(*EventFunction_t)(void*,int);
    typedef struct {
        CallbackFunction_t SetupCallback;
        CallbackFunction_t LoopCallback;
        CallbackFunction_t ExitCallback;
        EventFunction_t EventCallback;
    } PageList_TypeDef;
public:
    PageManager(uint8_t pageMax, uint8_t pageStackSize = 10);
    ~PageManager();
    uint8_t NowPage, LastPage, NextPage;
    uint8_t NewPage, OldPage;

    bool PageRegister(
        uint8_t pageID,
        CallbackFunction_t setupCallback,
        CallbackFunction_t loopCallback,
        CallbackFunction_t exitCallback,
        EventFunction_t eventCallback
    );
    bool PageClear(uint8_t pageID);
    bool PagePush(uint8_t pageID);
    bool PagePop();
    void PageChangeTo(uint8_t pageID);
    void PageEventTransmit(void* obj, int event);
    void PageStackClear();
    void Running();
private:
    PageList_TypeDef* PageList;
    uint8_t *PageStack;
    uint8_t PageStackSize;
    uint8_t PageStackTop;
    uint8_t MaxPage;
    bool IsPageBusy;
};

#endif
