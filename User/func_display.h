#ifndef __FUNC_DISPLAY_H__
#define __FUNC_DISPLAY_H__

#include "main.h"

typedef enum
{
    ARM_LED_NONE, // ȫ��
    ARM_LED_ALL, // ȫ��
    ARM_LED_ACTIVATED, // ����ɹ�
    ARM_LED_WAITING, // �ȴ�����
} ARM_Mode_E;

typedef enum
{
    GEN_ALL,      // ȫ������
    GEN_CORE,     // �������ư�
    GEN_RIM_CORE, // �������߿�
    GEN_AM,       // ����װ��
    GEN_RIM_AM,       // ����װ�ױ߿�
} GenType_E;

typedef struct
{
    char R;
    char G;
    char B;
} RGBData_S;

extern RGBData_S *RGBNowFix;
extern RGBData_S *RGBNow;

// �ư����ɫ
extern RGBData_S All_RGB_Set_Frame1;
extern RGBData_S All_RGB_Set_Frame2;
extern RGBData_S All_RGB_Set_Frame3;

// �����ɫ
extern RGBData_S RedFix_RGB_Set;
extern RGBData_S BlueFix_RGB_Set;

// ������ɫ
extern RGBData_S Red_RGB_Set;
extern RGBData_S Blue_RGB_Set;

// �ص� RGB = 0 0 0
extern RGBData_S None_RGB_Set;

// WAITINGģʽ�� ֡������
extern void LEDMODE_WAITING_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

// LEDALL/LEDNONEģʽ�� ֡������
extern void LEDMODE_ALL_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

// ACTIVATEDģʽ�� ֡������
extern void LEDMODE_ACTIVATED_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

#endif
