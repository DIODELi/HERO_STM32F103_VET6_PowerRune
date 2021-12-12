#ifndef __FUNC_ARMSETTINGS_H__
#define __FUNC_ARMSETTINGS_H__

#define ARM_MAINPIX_COUNT 427
#define ARM_ARMOURPIX_COUNT 8
#define ARM_OUTLINE_COUNT 153

#define ARM_MAIN_PERIOD 8
#define ARM_MAIN_WTH 2 // 箭头粗细

// 默认颜色设置
#define USE_RED 0
#define USE_FIX 1

// 各扇叶主边框与装甲边框交界点的序号，
// 增大此值可以让显示出的实际交界点更向装甲板方向移动
#define ARM1_DIVPOS 587
#define ARM2_DIVPOS 587
#define ARM3_DIVPOS 587
#define ARM4_DIVPOS 587
#define ARM5_DIVPOS 587

// 装甲板识别去抖动
#define SENSOR_CONFIRM_MAX 15 // 去抖动次数
#define SENSOR_RELEASE_LOAD 100 // 触发释抑（单位 ms）

// 流水灯移动一个像素对应时间 ms
#define FLOW_PERIOD 60

#endif
