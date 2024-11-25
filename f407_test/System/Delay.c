#include "Delay.h"

static uint8_t fac_us = 0; // ����������
void Delay_Init(uint8_t SysCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8); // ����SysTickʱ��Դ��HCLK
      fac_us = SysCLK / 8;                                  // ���ü���������
}

/// @brief nus��ʱ
/// @param nus ��ʱ��nus��
void Delay_us(uint32_t nus)
{
      uint32_t temp;
      SysTick->LOAD = nus * fac_us - 1;         // ����ֵ����
      SysTick->VAL = 0x00;                      // ��ռ�����
      SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // ��ʼ����
      do
      {
            temp = SysTick->CTRL;                     // ��ȡ���ƼĴ���״̬
      } while ((temp & 0x01) && !(temp & (1 << 16))); // temp&0x01:��ʱ��ʹ�ܣ�!(temp&(1<<16)):��ʱ������ֵ��Ϊ0
      SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      // �رռ���
      SysTick->VAL = 0x00;                            // ��ռ�����
}

/// @brief nms��ʱ
/// @param nus ��ʱ��ms��
void Delay_ms(uint32_t nms)
{
      uint32_t repeat = nms / 540;
      uint32_t remain = nms % 540;
      while (repeat)
      {
            Delay_us(540 * 1000); // ��ʱ 540 ms
            repeat--;
      }
      if (remain)
      {
            Delay_us(remain * 1000); // ��ʱremain ms
      }
}
