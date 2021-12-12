#ifndef __FUNC_SENSE_H__
#define __FUNC_SENSE_H__

#include "main.h"

typedef struct
{
    char Press;       // ����Ϊ1������Ϊ0
    int ConfirmCount; // ȥ������
    int ConfirmMax;   // ȥ����ֵ
    int ReleaseCount_Set; // �������� ������Ϊ����ֵ����0�Լ�����Ϊ0��ż����һ�δ���
    int ReleaseCount_Now;
    uint16_t GPIO_Pin_In;
} Key_S;

extern void Key_Init(Key_S *Key, int ConfirmMax, int ReleaseCount, uint16_t GPIO_Pin);

// ��¼����״̬
extern void Key_Rec(Key_S *Key, char Press_In);

// ����ͨ����Clear����1 ������������Ϊ��ѯ��1����� ����ϴ�������ʹ��
extern int8_t Key_GetPress(Key_S *Key, char Clear);

#endif
