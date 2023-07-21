/**
 ******************************************************************************
 * @file    GPIO/GPIO_Toggle/main.c
 * @author  MCD Application Team
 * @version V1.1.3
 * @date    14-December-2021
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f30x.h"
#include "stm32f30x_conf.h"

/** @addtogroup STM32F30x_StdPeriph_Examples
 * @{
 */

/** @addtogroup GPIO_Toggle
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void RCC_Configuration(void);
void led_out_gpio_config(void);

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	// SystemInit() 在 main 函数执行前由启动文件以汇编语言调用

	RCC_Configuration();
	led_out_gpio_config();

	while (1)
	{
		/* Set PC13 */
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);

		Delay(100);

		/* Reset PC13 */
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

		Delay(100);
	}
}

/**
 * @brief  设置系统时钟并启动系统滴答定时器 SysTick
 * @param  无
 * @retval 无
 */
void RCC_Configuration(void)
{
	//SystemInit(); // 在 system_stm32f303cctx.s 启动文件中已调用

	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}

	/**
	 * STM32关于优先级设定的理解 NVIC_SetPriority()
	 * https://blog.csdn.net/u010173859/article/details/10232393
	 * (1<<__NVIC_PRIO_BITS) -  1); 表示的是可用的最低优先级（1<<4为16，—1为15），
	 * 将后面那个 红色的 1 改为其他值即可改变此模块占先优先级
	 */
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); // 表示的是可用的最低优先级
}


/**
 * @brief  设置 PC13 LED 引脚为开漏无上拉模式
 * @param  无
 * @retval 无
 */
void led_out_gpio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOE Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure PE14 and PE15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	   // 开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   // 不上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds.
 * @retval None
 */
void Delay(__IO uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/**
 * @brief  Decrements the TimingDelay variable.
 * @param  None
 * @retval None
 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

