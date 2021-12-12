#include "func_Sense.h"

// ��ʼ��һ������
void Key_Init(Key_S *Key, int ConfirmMax, int ReleaseCount, uint16_t GPIO_Pin)
{
    Key->ConfirmCount = 0;
    Key->ConfirmMax = ConfirmMax;
    Key->Press = 0;
    Key->ReleaseCount_Set = ReleaseCount;
    Key->ReleaseCount_Now = 0;
    Key->GPIO_Pin_In = GPIO_Pin;
}

// ��¼����״̬
void Key_Rec(Key_S* Key, char Press_In)
{
    char toggle; // ��¼�Ƿ�Ҫ��ת

    if (Key->Press)
    {
        if (Press_In)
        {
            toggle = 0;
        }
        else
        {
            toggle = 1;
        }
    }
    else
    {
        if (Key->ReleaseCount_Now > 0)
        { // ����д������֣����ݲ�����
            Key->ReleaseCount_Now--;
            return;
        }
        if (Press_In)
        {
            toggle = 1;
        }
        else
        {
            toggle = 0;
        }
    }

    if (toggle)
    {
        // ���ֱ仯
        Key->ConfirmCount++;
        if (Key->ConfirmCount >= Key->ConfirmMax)
        {
            Key->Press = !Key->Press;
            Key->ConfirmCount = 0;
        }
        if(Key->Press)
        {
            // ���ô�������
            Key->ReleaseCount_Now = Key->ReleaseCount_Set;
        }
    }
    else
    {
        // û�г��ֱ仯
        if (Key->ConfirmCount > 0)
        {
            Key->ConfirmCount--;
        }
    }
}

// ����ͨ����Clear����1 ������������Ϊ��ѯ��1����� ����ϴ�������ʹ��
int8_t Key_GetPress(Key_S* Key, char Clear)
{
    if(Clear)
    {
        if (Key->Press)
        {
            Key->Press = 0;
            return 1;
        }
    }
    return Key->Press;
}
