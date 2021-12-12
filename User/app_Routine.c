#include "app_Routine.h"

#include "mod_Arm.h"
#include "func_Sense.h"

#include "func_ARMSettings.h"

// ��ȡ������GPIO����
static void Key_Fresh(void)
{
    int fori;
    for (fori = 0; fori < 5; fori++)
    {
        // ˢ������״̬
        Key_Rec(&ARMs[fori].Sensor, HAL_GPIO_ReadPin(AM_1_GPIO_Port, ARMs[fori].Sensor.GPIO_Pin_In));
    }
}

// 1ms ��������
static void Routine_1ms()
{
    static uint8_t frame = 0;
    Key_Fresh();
    if (Key_GetPress(&ARMs[0].Sensor,1))
    {
        // ����д�������
        if(ARMs[0].ARMMode == ARM_LED_NONE)
        {
            ARMs[0].ARMMode = ARM_LED_WAITING;
        }
        else if (ARMs[0].ARMMode == ARM_LED_WAITING)
        {
            ARMs[0].ARMMode = ARM_LED_ACTIVATED;
        }
        else
        {
            ARMs[0].ARMMode = ARM_LED_NONE;
        }
    }
    if (HAL_GetTick() % FLOW_PERIOD == 0)
    {
        frame++;
        if(frame>7)
        {
            frame = 0;
        }
        ARMs[0].FrameNow = frame;
    }
}

// ��ʱ�������ж�
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim6)
    {
        Routine_1ms();
    }
}
