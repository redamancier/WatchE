#include "DigitalFilter.h"

/*��ֵ�˲�����ȡ���13������ȥ�����ֵ����Сֵ��ʣ������ƽ��*/
FilterAverage<uint16_t> ADC_FA(13);

/*�ͻ��˲�����ȥ��50���µĲ���*/
FilterHysteresis<uint16_t> ADC_FH(50);

/*ADC��������*/
#define Sampling_Pin PA0

void setup()
{
    pinMode(Sampling_Pin, INPUT_ANALOG);
    Serial.begin(115200);
}

void loop()
{
    uint16_t adc_val = analogRead(Sampling_Pin);
    Serial.printf(
        "ADC Val:%d, FilterAverage:%d, FilterHysteresis:%d\r\n", 
        adc_val, 
        ADC_FA.getNext(adc_val),
        ADC_FH.getNext(adc_val)
    );
}
