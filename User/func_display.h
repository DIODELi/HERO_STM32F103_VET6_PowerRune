#ifndef __FUNC_DISPLAY_H__
#define __FUNC_DISPLAY_H__

#include "main.h"

typedef enum
{
    ARM_LED_NONE, // 全灭
    ARM_LED_ALL, // 全亮
    ARM_LED_ACTIVATED, // 激活成功
    ARM_LED_WAITING, // 等待激活
} ARM_Mode_E;

typedef enum
{
    GEN_ALL,      // 全部生成
    GEN_CORE,     // 生成主灯板
    GEN_RIM_CORE, // 生成主边框
    GEN_AM,       // 生成装甲
    GEN_RIM_AM,       // 生成装甲边框
} GenType_E;

typedef struct
{
    char R;
    char G;
    char B;
} RGBData_S;

extern RGBData_S *RGBNowFix;
extern RGBData_S *RGBNow;

// 灯板测试色
extern RGBData_S All_RGB_Set_Frame1;
extern RGBData_S All_RGB_Set_Frame2;
extern RGBData_S All_RGB_Set_Frame3;

// 青橙配色
extern RGBData_S RedFix_RGB_Set;
extern RGBData_S BlueFix_RGB_Set;

// 红蓝配色
extern RGBData_S Red_RGB_Set;
extern RGBData_S Blue_RGB_Set;

// 关灯 RGB = 0 0 0
extern RGBData_S None_RGB_Set;

// WAITING模式下 帧的生成
extern void LEDMODE_WAITING_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

// LEDALL/LEDNONE模式下 帧的生成
extern void LEDMODE_ALL_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

// ACTIVATED模式下 帧的生成
extern void LEDMODE_ACTIVATED_Gen(char BitNum, char Frame, GenType_E GenType, int CoreRimLEN);

#endif
