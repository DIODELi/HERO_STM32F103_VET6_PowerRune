#include "mod_Arm.h"
#include "func_ARMSettings.h"
#include "func_display.h"

ARM_S ARMs[5]; // 定义5个扇叶

static char UseRed = USE_RED; // 默认为红色能量机关
static char UseFix = USE_FIX; // 默认为青橙配色

// 比较RGB是否相同 不同返回0
static char RGBcmp(RGBData_S *RGB1, RGBData_S *RGB2)
{
    if (RGB1->R != RGB2->R)
        return 1;
    if (RGB1->G != RGB2->G)
        return 1;
    if (RGB1->B != RGB2->B)
        return 1;
    return 0;
}

static void RGBcpy(RGBData_S *RGB1, RGBData_S *RGB2)
{
    RGB1->B = RGB2->B;
    RGB1->G = RGB2->G;
    RGB1->R = RGB2->R;
}

static void Location_Init(Location_S *Loc, int StartPos, int EndPos)
{
    Loc->StartPos = StartPos;
    Loc->EndPos = EndPos;
    Loc->Len = EndPos - StartPos + 1;
    if(Loc->Len <= 0)
    {
        while (1)
        {
            ; // 输入起始位置大于等于结束位置
        }
    }
}

// 初始化通用的基本信息
static void ARMData_PreInit()
{
    ARM_S *ArmNow;
    int fori;

    for (fori = 0; fori < 5; fori++)
    {
        ArmNow = &ARMs[fori];

        ArmNow->AM.Frame = 0;
        ArmNow->AM.Mode = LED_OFF;
        ArmNow->AM.RGB.R = 0;
        ArmNow->AM.RGB.G = 0;
        ArmNow->AM.RGB.B = 0;

        ArmNow->Rim_AM.Frame = 0;
        ArmNow->Rim_AM.Mode = LED_OFF;
        ArmNow->Rim_AM.RGB.R = 0;
        ArmNow->Rim_AM.RGB.G = 0;
        ArmNow->Rim_AM.RGB.B = 0;

        ArmNow->Core.Frame = 0;
        ArmNow->Core.Mode = LED_OFF;
        ArmNow->Core.RGB.R = 0;
        ArmNow->Core.RGB.G = 0;
        ArmNow->Core.RGB.B = 0;

        ArmNow->Rim_Core.Frame = 0;
        ArmNow->Rim_Core.Mode = LED_OFF;
        ArmNow->Rim_Core.RGB.R = 0;
        ArmNow->Rim_Core.RGB.G = 0;
        ArmNow->Rim_Core.RGB.B = 0;

        ArmNow->ARMMode = ARM_LED_NONE;
    }

    Key_Init(&ARMs[0].Sensor, SENSOR_CONFIRM_MAX, SENSOR_RELEASE_LOAD, AM_1_Pin);
    Key_Init(&ARMs[1].Sensor, SENSOR_CONFIRM_MAX, SENSOR_RELEASE_LOAD, AM_2_Pin);
    Key_Init(&ARMs[2].Sensor, SENSOR_CONFIRM_MAX, SENSOR_RELEASE_LOAD, AM_3_Pin);
    Key_Init(&ARMs[3].Sensor, SENSOR_CONFIRM_MAX, SENSOR_RELEASE_LOAD, AM_4_Pin);
    Key_Init(&ARMs[4].Sensor, SENSOR_CONFIRM_MAX, SENSOR_RELEASE_LOAD, AM_5_Pin);
}

// 初始化各扇叶数据
void ARMData_Load()
{
    ARMData_PreInit();

    Location_Init(&ARMs[0].Core.Location, 0, 426);
    Location_Init(&ARMs[0].AM.Location, 427, 434);
    Location_Init(&ARMs[0].Rim_Core.Location, 435, ARM1_DIVPOS);
    Location_Init(&ARMs[0].Rim_AM.Location, ARM1_DIVPOS+1, 750);
    ARMs[0].BitNum = 0;

    Location_Init(&ARMs[1].Core.Location, 0, 426);
    Location_Init(&ARMs[1].AM.Location, 427, 434);
    Location_Init(&ARMs[1].Rim_Core.Location, 435, ARM2_DIVPOS);
    Location_Init(&ARMs[1].Rim_AM.Location, ARM2_DIVPOS + 1, 750);
    ARMs[1].BitNum = 1;

    Location_Init(&ARMs[2].Core.Location, 0, 426);
    Location_Init(&ARMs[2].AM.Location, 427, 434);
    Location_Init(&ARMs[2].Rim_Core.Location, 435, ARM3_DIVPOS);
    Location_Init(&ARMs[2].Rim_AM.Location, ARM3_DIVPOS + 1, 750);
    ARMs[2].BitNum = 3;

    Location_Init(&ARMs[3].Core.Location, 0, 426);
    Location_Init(&ARMs[3].AM.Location, 427, 434);
    Location_Init(&ARMs[3].Rim_Core.Location, 435, ARM4_DIVPOS);
    Location_Init(&ARMs[3].Rim_AM.Location, ARM4_DIVPOS + 1, 750);
    ARMs[3].BitNum = 4;

    Location_Init(&ARMs[4].Core.Location, 0, 426);
    Location_Init(&ARMs[4].AM.Location, 427, 434);
    Location_Init(&ARMs[4].Rim_Core.Location, 435, ARM5_DIVPOS);
    Location_Init(&ARMs[4].Rim_AM.Location, ARM5_DIVPOS + 1, 750);
    ARMs[4].BitNum = 5;
}

// 生成指定扇叶的图形
void ARMGen(ARM_S *ArmNow)
{
    int CoreRimLen;
    if (UseRed)
    {
        RGBNow = &Red_RGB_Set;
        if (UseFix)
        {
            RGBNowFix = &RedFix_RGB_Set;
        }
        else
        {
            RGBNowFix = &Red_RGB_Set;
        }
    }
    else
    {
        RGBNow = &Blue_RGB_Set;
        if (UseFix)
        {
            RGBNowFix = &BlueFix_RGB_Set;
        }
        else
        {
            RGBNowFix = &Blue_RGB_Set;
        }
    }

    CoreRimLen = ArmNow->Rim_Core.Location.Len;

    switch (ArmNow->ARMMode)
    {
    default:
    case ARM_LED_NONE:
        // 全亮纯色
        RGBNow = &None_RGB_Set;
        if (ArmNow->Core.Mode != LED_ON || RGBcmp(&ArmNow->Core.RGB, &None_RGB_Set))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_CORE, CoreRimLen);
            ArmNow->Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Core.RGB, &None_RGB_Set);
        }
        if (ArmNow->AM.Mode != LED_ON || RGBcmp(&ArmNow->AM.RGB, &None_RGB_Set))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_AM, CoreRimLen);
            ArmNow->AM.Mode = LED_ON;
            RGBcpy(&ArmNow->AM.RGB, &None_RGB_Set);
        }
        if (ArmNow->Rim_Core.Mode != LED_ON || RGBcmp(&ArmNow->Rim_Core.RGB, &None_RGB_Set))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_CORE, CoreRimLen);
            ArmNow->Rim_Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_Core.RGB, &None_RGB_Set);
        }
        if (ArmNow->Rim_AM.Mode != LED_ON || RGBcmp(&ArmNow->Rim_AM.RGB, &None_RGB_Set))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_AM, CoreRimLen);
            ArmNow->Rim_AM.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_AM.RGB, &None_RGB_Set);
        }
        break;
    case ARM_LED_ALL:
        // 全亮纯色
        if (ArmNow->Core.Mode != LED_ON || RGBcmp(&ArmNow->Core.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_CORE, CoreRimLen);
            ArmNow->Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Core.RGB, RGBNow);
        }
        if (ArmNow->AM.Mode != LED_ON || RGBcmp(&ArmNow->AM.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_AM, CoreRimLen);
            ArmNow->AM.Mode = LED_ON;
            RGBcpy(&ArmNow->AM.RGB, RGBNow);
        }
        if (ArmNow->Rim_Core.Mode != LED_ON || RGBcmp(&ArmNow->Rim_Core.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_CORE, CoreRimLen);
            ArmNow->Rim_Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_Core.RGB, RGBNow);
        }
        if (ArmNow->Rim_AM.Mode != LED_ON || RGBcmp(&ArmNow->Rim_AM.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ALL_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_AM, CoreRimLen);
            ArmNow->Rim_AM.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_AM.RGB, RGBNow);
        }
        break;
    case ARM_LED_ACTIVATED:
        // 全亮(装甲板独立颜色)
        if (ArmNow->Core.Mode != LED_ON || RGBcmp(&ArmNow->Core.RGB, RGBNowFix))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ACTIVATED_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_CORE, CoreRimLen);
            ArmNow->Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Core.RGB, RGBNowFix);
        }
        if (ArmNow->AM.Mode != LED_ON || RGBcmp(&ArmNow->AM.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ACTIVATED_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_AM,CoreRimLen);
            ArmNow->AM.Mode = LED_ON;
            RGBcpy(&ArmNow->AM.RGB, RGBNow);
        }
        if (ArmNow->Rim_Core.Mode != LED_ON || RGBcmp(&ArmNow->Rim_Core.RGB, RGBNowFix))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ACTIVATED_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_CORE, CoreRimLen);
            ArmNow->Rim_Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_Core.RGB, RGBNowFix);
        }
        if (ArmNow->Rim_AM.Mode != LED_ON || RGBcmp(&ArmNow->Rim_AM.RGB, RGBNowFix))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_ACTIVATED_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_AM, CoreRimLen);
            ArmNow->Rim_AM.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_AM.RGB, RGBNowFix);
        }
        break;
    case ARM_LED_WAITING:
        // 流水箭头
        if (ArmNow->Core.Mode != LED_FRAME || ArmNow->Core.Frame != ArmNow->FrameNow || RGBcmp(&ArmNow->Core.RGB, RGBNowFix))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_WAITING_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_CORE, CoreRimLen);
            ArmNow->Core.Mode = LED_FRAME;
            ArmNow->Core.Frame = ArmNow->FrameNow;
            RGBcpy(&ArmNow->Core.RGB, RGBNowFix);
        }
        if (ArmNow->AM.Mode != LED_ON || RGBcmp(&ArmNow->AM.RGB, RGBNow))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_WAITING_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_AM, CoreRimLen);
            ArmNow->AM.Mode = LED_ON;
            RGBcpy(&ArmNow->AM.RGB, RGBNow);
        }
        if (ArmNow->Rim_Core.Mode != LED_ON || RGBcmp(&ArmNow->Rim_Core.RGB, &None_RGB_Set))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_WAITING_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_CORE, CoreRimLen);
            ArmNow->Rim_Core.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_Core.RGB, &None_RGB_Set);
        }
        if (ArmNow->Rim_AM.Mode != LED_ON || RGBcmp(&ArmNow->Rim_AM.RGB, RGBNowFix))
        { // 需要修改形状或者需要修改颜色
            LEDMODE_WAITING_Gen(ArmNow->BitNum, ArmNow->FrameNow, GEN_RIM_AM, CoreRimLen);
            ArmNow->Rim_AM.Mode = LED_ON;
            RGBcpy(&ArmNow->Rim_AM.RGB, RGBNowFix);
        }
        break;
    }
}

// 修改颜色设置
void Color_Set(char UseRed_Set, char UseFix_Set)
{
    if (UseRed_Set)
    {
        UseRed = 1;
    }
    else
    {
        UseRed = 0;
    }
    if (UseFix_Set)
    {
        UseFix = 1;
    }
    else
    {
        UseFix = 0;
    }
}
