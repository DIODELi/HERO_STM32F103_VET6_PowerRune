#include "func_display.h"

#include "drv_pix.h"
#include "drv_pix_Settings.h"
#include "func_ARMSettings.h"

static int PixMaxNum = PIX_COUNT_MAX_100 * 100 - 1; // ����������

// ��ģʽ��ɫ�����������趨
RGBData_S All_RGB_Set_Frame1 =
    {
        .R = 2,
        .G = 2,
        .B = 0};
RGBData_S All_RGB_Set_Frame2 =
    {
        .R = 2,
        .G = 0,
        .B = 2};
RGBData_S All_RGB_Set_Frame3 =
    {
        .R = 0,
        .G = 2,
        .B = 2};

RGBData_S None_RGB_Set =
    {
        .R = 0,
        .G = 0,
        .B = 0};

// �����ɫ
RGBData_S RedFix_RGB_Set =
    {
        .R = 93,
        .G = 7,
        .B = 0};
RGBData_S BlueFix_RGB_Set =
    {
        .R = 0,
        .G = 15,
        .B = 85};

// ������ɫ
RGBData_S Red_RGB_Set =
    {
        .R = 100,
        .G = 0,
        .B = 0};

RGBData_S Blue_RGB_Set =
    {
        .R = 0,
        .G = 0,
        .B = 100};

RGBData_S *RGBNowFix;
RGBData_S *RGBNow;

// ����ָ����LED��ʱ�Ƿ���Ҫ�� Num��0������Count��1����
static char Flow_Gen(int PixNum, char Frame)
{
    int Line; // ������(��0����)
    int PixPOS; // ����Ϊ�ڼ�������1������
    int imgPOS; // ����Ϊ����һάͼ���������ϵĵڼ�������1������
    // ���Խ�����
    // if (PixNum >= ARM_MAINPIX_COUNT)
    // {
    //     return 0;
    // }
    Line = PixNum / 61;
    if(Line%2!=0)
    {
        PixPOS = 61 * (Line + 1) - PixNum - 1; // ż���е�˳���෴
        Line -= 1; // �෴��Line��ȡ��һ�е�ֵ
    }
    else
    {
        PixPOS = PixNum;
    }
    PixPOS %= 61;

    imgPOS = (61 - PixPOS) + ARM_MAIN_PERIOD*2 - Line / 2 - Frame;
    // ��������ͼ��
    if ((imgPOS + ARM_MAIN_PERIOD - 1) % ARM_MAIN_PERIOD < ARM_MAIN_WTH)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// ��ͷ��亯��
static int Flow_Fill(char BitNum, char Frame, RGBData_S *RGB)
{
    int Pix = 0;
    while (Pix < ARM_MAINPIX_COUNT)
    {
        if (Flow_Gen(Pix, Frame))
        {
            Data_Write_Pix(Pix, BitNum, RGB->R, RGB->G, RGB->B);
        }
        else
        {
            Data_Write_Pix(Pix, BitNum, 0, 0, 0);
        }
        Pix++;
    }
    return Pix;
}

// ��ɫ��亯��
static int Color_Fill(char BitNum, int PixStart, int Len, RGBData_S *RGB)
{
    int Pix = PixStart;
    int End = Len + Pix;
    while (Pix < End)
    {
        Data_Write_Pix(Pix, BitNum, RGB->R, RGB->G, RGB->B);
        Pix++;
    }
    return Pix;
}

// WAITINGģʽ�� ֡������
void LEDMODE_WAITING_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN)
{
    int Pix = 0;

    switch (GenType)
    {
    default:
    case GEN_ALL:
        Pix = Flow_Fill(BitNum, Frame, RGBNowFix);                 // Core GEN
        Pix = Color_Fill(BitNum, Pix, 8, RGBNow);                  // AM GEN
        Pix = Color_Fill(BitNum, Pix, CoreRimLEN, &None_RGB_Set);   // Core Rim GEN
        Pix = Color_Fill(BitNum, Pix, PixMaxNum - Pix, RGBNowFix); // AM Rim GEN
        break;
    case GEN_CORE:
        Flow_Fill(BitNum, Frame, RGBNowFix); // Core GEN
        break;
    case GEN_AM:
        Pix = ARM_MAINPIX_COUNT;
        Color_Fill(BitNum, Pix, ARM_ARMOURPIX_COUNT, RGBNow); // AM GEN
        break;
    case GEN_RIM_CORE:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT;
        Color_Fill(BitNum, Pix, CoreRimLEN, &None_RGB_Set); // Core Rim GEN
        break;
    case GEN_RIM_AM:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT + CoreRimLEN;
        Color_Fill(BitNum, Pix, PixMaxNum - Pix, RGBNowFix); // AM Rim GEN
        break;
    }
}

// LEDALL/LEDNONEģʽ�� ֡������
void LEDMODE_ALL_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN)
{
    int Pix = 0;

    switch (GenType)
    {
    default:
    case GEN_ALL:
        Color_Fill(BitNum, 0, PixMaxNum, RGBNow);
        break;
    case GEN_CORE:
        Color_Fill(BitNum, 0, ARM_MAINPIX_COUNT, RGBNow); // Core GEN
        break;
    case GEN_AM:
        Pix = ARM_MAINPIX_COUNT;
        Color_Fill(BitNum, Pix, ARM_ARMOURPIX_COUNT, RGBNow); // AM GEN
        break;
    case GEN_RIM_CORE:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT;
        Color_Fill(BitNum, Pix, CoreRimLEN, RGBNow); // Core Rim GEN
        break;
    case GEN_RIM_AM:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT + CoreRimLEN;
        Color_Fill(BitNum, Pix, PixMaxNum - Pix, RGBNow); // AM Rim GEN
        break;
    }
}

// ACTIVATEDģʽ�� ֡������
void LEDMODE_ACTIVATED_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN)
{
    int Pix = 0;

    switch (GenType)
    {
    default:
    case GEN_ALL:
        Pix = Color_Fill(BitNum, 0, ARM_MAINPIX_COUNT, RGBNowFix);
        Pix = Color_Fill(BitNum, Pix, ARM_ARMOURPIX_COUNT, RGBNow); // AM GEN
        Pix = Color_Fill(BitNum, Pix, CoreRimLEN, RGBNowFix);       // Core Rim GEN
        Color_Fill(BitNum, Pix, PixMaxNum - Pix, RGBNowFix);        // AM Rim GEN
        break;
    case GEN_CORE:
        Color_Fill(BitNum, 0, ARM_MAINPIX_COUNT, RGBNowFix); // Core GEN
        break;
    case GEN_AM:
        Pix = ARM_MAINPIX_COUNT;
        Color_Fill(BitNum, Pix, ARM_ARMOURPIX_COUNT, RGBNow); // AM GEN
        break;
    case GEN_RIM_CORE:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT;
        Color_Fill(BitNum, Pix, CoreRimLEN, RGBNowFix); // Core Rim GEN
        break;
    case GEN_RIM_AM:
        Pix = ARM_MAINPIX_COUNT + ARM_ARMOURPIX_COUNT + CoreRimLEN;
        Color_Fill(BitNum, Pix, PixMaxNum - Pix, RGBNowFix); // AM Rim GEN
        break;
    }
}
