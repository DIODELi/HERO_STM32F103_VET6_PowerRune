#ifndef __DRV_PIX_H__
#define __DRV_PIX_H__

#include "main.h"

// 初始化发送数据
extern void Data_Init(void);

// 写入一个像素的数据(G-R-B)
extern void Data_Write_Pix(int PixNum, char BitNum, char Red, char Green, char Blue);

// 信号输出
extern HAL_StatusTypeDef HAL_TIM_DMA_Send_Start(void);
// 等待发送完成 如果没在发送则直接退出 返回值为发送时长 ms
extern int WaitForSending(void);

// 复位GPIO，停止DMA，在开启连续发送时可以用于关闭发送
extern void Send_OFF(void);

#endif
