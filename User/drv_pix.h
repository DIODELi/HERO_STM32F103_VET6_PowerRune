#ifndef __DRV_PIX_H__
#define __DRV_PIX_H__

#include "main.h"

// ��ʼ����������
extern void Data_Init(void);

// д��һ�����ص�����(G-R-B)
extern void Data_Write_Pix(int PixNum, char BitNum, char Red, char Green, char Blue);

// �ź����
extern HAL_StatusTypeDef HAL_TIM_DMA_Send_Start(void);
// �ȴ�������� ���û�ڷ�����ֱ���˳� ����ֵΪ����ʱ�� ms
extern int WaitForSending(void);

// ��λGPIO��ֹͣDMA���ڿ�����������ʱ�������ڹرշ���
extern void Send_OFF(void);

#endif
