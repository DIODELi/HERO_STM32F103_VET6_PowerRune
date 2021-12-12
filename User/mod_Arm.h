#ifndef __MOD_ARM_H__
#define __MOD_ARM_H__

#include "main.h"
#include "func_display.h"

#include "func_Sense.h"

typedef enum
{
    LED_ON,    // 全亮
    LED_OFF,   // 全灭
    LED_FRAME, // 复杂图形
} LED_Mode_E;

typedef struct
{
    int StartPos;
    int EndPos;
    int Len; // 该段LED个数
} Location_S; // 一段LED的位置

typedef struct 
{
    LED_Mode_E Mode; // 该区域当前的模式
    char Frame; // 该区域当前的帧标号
    RGBData_S RGB; // RGB
    Location_S Location; // 区域位置
} LEDArea_S; // 一个区域的数据状态

typedef struct 
{
    ARM_Mode_E ARMMode; // 扇叶整体模式
    LEDArea_S Core; // 主灯板区
    LEDArea_S AM; // 装甲板区
    LEDArea_S Rim_Core; // 主边框
    LEDArea_S Rim_AM; // 装甲边框
    Key_S Sensor;
    char BitNum; // 对应GPIO输出位
    int FrameNow; // 当前运行到的帧序号
} ARM_S;

extern ARM_S ARMs[5]; // 定义5个扇叶

// 生成指定扇叶的图形
extern void ARMGen(ARM_S *ArmNow);

// 修改颜色设置
extern void Color_Set(char UseRed_Set, char UseFix_Set);

// 初始化各扇叶数据
extern void ARMData_Load(void);

#endif
