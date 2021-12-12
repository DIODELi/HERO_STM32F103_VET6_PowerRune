#include "drv_pix.h"
#include "drv_pix_Settings.h"

extern TIM_HandleTypeDef htim1;

// ���ڴ�����շ������ݵĲ�������
#if CONTINUOUS_SEND
// ���200������ȫ0����������������ʱ����֮֡��ĳ����͵�ƽ
char Send_All[PIX_COUNT_MAX_100 * 100 * 24 * 3 + 200] = {0};
#else
char Send_All[PIX_COUNT_MAX_100 * 100 * 24 * 3] = {0};
#endif

const int MaxCount = PIX_COUNT_MAX_100 * 100 * 24 * 3;

char SendingFlag = 0;

static const int32_t DataLen_Half = (PIX_COUNT_MAX_100 * 100 * 24 * 3 + 200) / 2;
static const int32_t DataLen_Total = (PIX_COUNT_MAX_100 * 100 * 24 * 3 + 200);

// ���DMA���ڴ���ĵ�ַ�Ƿ�ӽ���ǰλ��
static void DMA_Address_Wait(int Addr_Wr)
{
    uint32_t Addr_DMA;
    int64_t Addr_SUBB;

    __disable_irq(); // ���жϱ�֤ʵʱ��
restart:
    Addr_DMA = DataLen_Total - hdma_tim1_up.Instance->CNDTR;
    Addr_SUBB = Addr_DMA - Addr_Wr;

    // ��Ȧ����
    if (Addr_SUBB > DataLen_Half)
    {
        Addr_SUBB -= DataLen_Total;
    }
    else if (Addr_SUBB < -DataLen_Half)
    {
        Addr_SUBB += DataLen_Total;
    }

    if(Addr_SUBB<0)
    { // ����ֵ
        Addr_SUBB = -Addr_SUBB;
    }
    if(Addr_SUBB<500)
    { // ��̫����ѭ���ȴ�
        __enable_irq();
        goto restart;
    }
}

// ��շ�������
void Data_Init()
{
    int fori;
    for (fori = 0; fori < MaxCount; fori++)
    {
        if(fori%3==0)
        {
            Send_All[fori] = 0xff; // ÿ3���ĵ�һ��һ��Ϊ1����Ϊ��ʱһ����Ҫ�ӵ͵�ƽ��ת���ߵ�ƽ
        }
        else
        {
            Send_All[fori] = 0;
        }
    }

#if CONTINUOUS_SEND
    for (; fori < MaxCount + 200;fori++)
    {
        Send_All[fori] = 0; // �����Ҫ�������ͣ���ʹ���ڴ�+DMA���ɲ���ʱ��Ҫר�����һ�ε͵�ƽ
    }
#endif
}

// ��λд�����ݣ�����λ�ã�λλ�ã�1��0��
static void Data_Write_Bit(int WrNum, char BitNum, char Data)
{
    // if (WrNum >= MaxCount || BitNum>7)
    // {
    //     while (1)
    //     { // ����Խ���λԽ��
    //         ;
    //     }
    // }
    if(Data)
    {
        // ����1
        Send_All[WrNum] |= 1 << BitNum;
    }
    else
    {
        // ����0
        Send_All[WrNum] &= ~(1 << BitNum);
    }
}

// ���յ���ͨ��д������
static int Data_Write_Send(int PosNow, char BitNum, char DataToSend)
{
    if(DataToSend)
    {
        // ��1
        //Data_Write_Bit(PosNow, BitNum, 1);
        PosNow++;
        Data_Write_Bit(PosNow, BitNum, 1);
        PosNow++;
        //Data_Write_Bit(PosNow, BitNum, 0);
    }
    else
    {
        // ��0
        //Data_Write_Bit(PosNow, BitNum, 1);
        PosNow++;
        Data_Write_Bit(PosNow, BitNum, 0);
        PosNow++;
        //Data_Write_Bit(PosNow, BitNum, 0);
    }
    PosNow++;
    return PosNow; // ���ص�ǰд����ɺ��λ��
}

// д��һ�����ص�����(G-R-B)
void Data_Write_Pix(int PixNum,char BitNum,char Red,char Green,char Blue)
{
    int PosNow = PixNum * 3 * 24; // �����ض�ӦSend ALL�����е��׸��ֽڵ���ţ�������Ϊ�������У���ǰд�뵽��λ��
    int BitToSend; // ��0�뻹�Ƿ�1��
    register int fori;

    DMA_Address_Wait(PosNow); // ���ú��ر��жϣ�������Ҫ�ֶ��ؿ��ж�

    for (fori = 7; fori >= 0 ;fori--)
    {
        if (Green & (1 << fori))
        {
            BitToSend = 1;
        }
        else
        {
            BitToSend = 0;
        }
        PosNow = Data_Write_Send(PosNow, BitNum, BitToSend);
    }
    for (fori = 7; fori >= 0; fori--)
    {
        if (Red & (1 << fori))
        {
            BitToSend = 1;
        }
        else
        {
            BitToSend = 0;
        }
        PosNow = Data_Write_Send(PosNow, BitNum, BitToSend);
    }
    for (fori = 7; fori >= 0; fori--)
    {
        if (Blue & (1 << fori))
        {
            BitToSend = 1;
        }
        else
        {
            BitToSend = 0;
        }
        PosNow = Data_Write_Send(PosNow, BitNum, BitToSend);
    }

    __enable_irq(); // DMA_Address_Wait������жϣ������ؿ�
}

// ������ɣ���λGPIO����λ��־λ
void Send_OFF()
{
    HAL_TIM_Base_Stop_DMA(&htim1);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);
}

/**
  * @brief  TIM DMA Period Elapse complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
    TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

    if (htim->hdma[TIM_DMA_ID_UPDATE]->Init.Mode == DMA_NORMAL)
    {
        htim->State = HAL_TIM_STATE_READY;
    }

// #if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
//     htim->PeriodElapsedCallback(htim);
// #else
//     HAL_TIM_PeriodElapsedCallback(htim);
// #endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
#if (!CONTINUOUS_SEND)
    SendingFlag = 0;
    Send_OFF();
#endif
}

/**
  * @brief  TIM DMA Period Elapse half complete callback.
  * @param  hdma pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAPeriodElapsedHalfCplt(DMA_HandleTypeDef *hdma)
{
    TIM_HandleTypeDef *htim = (TIM_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
    htim->PeriodElapsedHalfCpltCallback(htim);
#else
    HAL_TIM_PeriodElapsedHalfCpltCallback(htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
}

/**
  * @brief  Starts the TIM Base generation in DMA mode.
  * @param  htim TIM Base handle
  * @param  pData The source Buffer address.
  * @param  Length The length of data to be transferred from memory to peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMA_Send_Start()
{
    uint32_t tmpsmcr;
    TIM_HandleTypeDef *htim = &htim1;
    uint32_t *pData = (uint32_t *)Send_All;
    uint16_t Length = MaxCount;

#if (CONTINUOUS_SEND)
    Length+=200;
#endif
    
    /* Check the parameters */
    assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

    WaitForSending();
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);
    SendingFlag = 1;

    /* Set the TIM state */
    if (htim->State == HAL_TIM_STATE_BUSY)
    {
        return HAL_BUSY;
    }
    else if (htim->State == HAL_TIM_STATE_READY)
    {
        if ((pData == NULL) && (Length > 0U))
        {
            return HAL_ERROR;
        }
        else
        {
            htim->State = HAL_TIM_STATE_BUSY;
        }
    }
    else
    {
        return HAL_ERROR;
    }

    /* Set the DMA Period elapsed callbacks */
    htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
    htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback = TIM_DMAPeriodElapsedHalfCplt;

    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError;

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)(&(GPIOD->ODR)),
                         Length) != HAL_OK)
    {
        /* Return error status */
        return HAL_ERROR;
    }

    /* Enable the TIM Update DMA request */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);

    /* Enable the Peripheral, except in trigger mode where enable is automatically done with trigger */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance))
    {
        tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
        if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr))
        {
            __HAL_TIM_ENABLE(htim);
        }
    }
    else
    {
        __HAL_TIM_ENABLE(htim);
    }

    /* Return function status */
    return HAL_OK;
}

// �ȴ�������� ���û�ڷ�����ֱ���˳� ����ֵΪ����ʱ�� ms
int WaitForSending()
{
    uint32_t TickStart;
    TickStart = HAL_GetTick();
    while(SendingFlag)
        ;
    return HAL_GetTick() - TickStart;
}
