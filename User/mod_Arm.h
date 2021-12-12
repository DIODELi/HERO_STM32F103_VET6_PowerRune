#ifndef __MOD_ARM_H__
#define __MOD_ARM_H__

#include "main.h"
#include "func_display.h"

#include "func_Sense.h"

typedef enum
{
    LED_ON,    // ȫ��
    LED_OFF,   // ȫ��
    LED_FRAME, // ����ͼ��
} LED_Mode_E;

typedef struct
{
    int StartPos;
    int EndPos;
    int Len; // �ö�LED����
} Location_S; // һ��LED��λ��

typedef struct 
{
    LED_Mode_E Mode; // ������ǰ��ģʽ
    char Frame; // ������ǰ��֡���
    RGBData_S RGB; // RGB
    Location_S Location; // ����λ��
} LEDArea_S; // һ�����������״̬

typedef struct 
{
    ARM_Mode_E ARMMode; // ��Ҷ����ģʽ
    LEDArea_S Core; // ���ư���
    LEDArea_S AM; // װ�װ���
    LEDArea_S Rim_Core; // ���߿�
    LEDArea_S Rim_AM; // װ�ױ߿�
    Key_S Sensor;
    char BitNum; // ��ӦGPIO���λ
    int FrameNow; // ��ǰ���е���֡���
} ARM_S;

extern ARM_S ARMs[5]; // ����5����Ҷ

// ����ָ����Ҷ��ͼ��
extern void ARMGen(ARM_S *ArmNow);

// �޸���ɫ����
extern void Color_Set(char UseRed_Set, char UseFix_Set);

// ��ʼ������Ҷ����
extern void ARMData_Load(void);

#endif
