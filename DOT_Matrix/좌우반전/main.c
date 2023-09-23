/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
unsigned char bit_flip(unsigned char number, int position);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define LD1071_DIN GPIO_PIN_0
#define LD1071_CLK GPIO_PIN_1
#define LD1071_STB GPIO_PIN_2
#define LD1071_PWM GPIO_PIN_5

uint16_t disp1ay[38][8]={
{0x003C,0x0042,0x0042,0x0042,0x0042,0x0042,0x0042,0x003C},//0
{0x0010,0x0030,0x0050,0x0010,0x0010,0x0010,0x0010,0x007c},//1
{0x007E,0x0002,0x0002,0x007E,0x0040,0x0040,0x0040,0x007E},//2
{0x003E,0x0002,0x0002,0x003E,0x0002,0x0002,0x003E,0x0000},//3
{0x0008,0x0018,0x0028,0x0048,0x00FE,0x0008,0x0008,0x0008},//4
{0x003C,0x0020,0x0020,0x003C,0x0004,0x0004,0x003C,0x0000},//5
{0x003C,0x0020,0x0020,0x003C,0x0024,0x0024,0x003C,0x0000},//6
{0x003E,0x0022,0x0004,0x0008,0x0008,0x0008,0x0008,0x0008},//7
{0x0000,0x003E,0x0022,0x0022,0x003E,0x0022,0x0022,0x003E},//8
{0x003E,0x0022,0x0022,0x003E,0x0002,0x0002,0x0002,0x003E},//9
{0x0018,0x0024,0x0042,0x0042,0x007E,0x0042,0x0042,0x0042},//A
{0x003C,0x0022,0x0022,0x003c,0x0022,0x0022,0x003C,0x0000},//B
{0x003C,0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x003C},//C
{0x007C,0x0022,0x0022,0x0022,0x0022,0x0022,0x0022,0x007C},//D
{0x007C,0x0040,0x0040,0x007C,0x0040,0x0040,0x0040,0x007C},//E
{0x007C,0x0040,0x0040,0x007C,0x0040,0x0040,0x0040,0x0040},//F
{0x003C,0x0040,0x0040,0x0040,0x004c,0x0044,0x0044,0x003C},//G
{0x0044,0x0044,0x0044,0x007C,0x0044,0x0044,0x0044,0x0044},//H
{0x007C,0x0010,0x0010,0x0010,0x0010,0x0010,0x0010,0x007C},//I
{0x003C,0x0008,0x0008,0x0008,0x0008,0x0008,0x0048,0x0030},//J
{0x0000,0x0024,0x0028,0x0030,0x0020,0x0030,0x0028,0x0024},//K
{0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x0040,0x007C},//L
{0x0081,0x00C3,0x00A5,0x0099,0x0081,0x0081,0x0081,0x0081},//M
{0x0000,0x0042,0x0062,0x0052,0x004A,0x0046,0x0042,0x0000},//N
{0x003C,0x0042,0x0042,0x0042,0x0042,0x0042,0x0042,0x003C},//O
{0x003C,0x0022,0x0022,0x0022,0x003C,0x0020,0x0020,0x0020},//P
{0x001C,0x0022,0x0022,0x0022,0x0022,0x0026,0x0022,0x001D},//Q
{0x003C,0x0022,0x0022,0x0022,0x003C,0x0024,0x0022,0x0021},//R
{0x0000,0x001E,0x0020,0x0020,0x003E,0x0002,0x0002,0x003C},//S
{0x0000,0x003E,0x0008,0x0008,0x0008,0x0008,0x0008,0x0008},//T
{0x0042,0x0042,0x0042,0x0042,0x0042,0x0042,0x0022,0x001C},//U
{0x0042,0x0042,0x0042,0x0042,0x0042,0x0042,0x0024,0x0018},//V
{0x0000,0x0049,0x0049,0x0049,0x0049,0x002A,0x001C,0x0000},//W
{0x0000,0x0041,0x0022,0x0014,0x0008,0x0014,0x0022,0x0041},//X
{0x0041,0x0022,0x0014,0x0008,0x0008,0x0008,0x0008,0x0008},//Y
{0x0000,0x007F,0x0002,0x0004,0x0008,0x0010,0x0020,0x007F},//Z
};
uint16_t reverseBits(uint16_t num)
{
	int NO_OF_BITS = sizeof(num) * 8;
	uint16_t reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((num & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}
uint16_t flipped_disp1ay[38][8];
void __flip()
{
	for(int i = 0; i < 38;i ++)
	{
		for(int j = 0; j < 8; j++)
		{
			flipped_disp1ay[i][j] = reverseBits(disp1ay[i][j]) >> 8;
		}
	}
}
unsigned char bit_flip(unsigned char number, int position) {
    unsigned char mask = 1 << position; // ?좏깮???꾩튂?????留덉뒪???앹꽦
    unsigned char flipped_number = number ^ mask; // XOR ?곗궛???ъ슜?섏뿬 鍮꾪듃 諛섏쟾

    return flipped_number;
}

void LD1071_Tx_CMD(unsigned int cmd)
{
	unsigned int i = 0;
	// Strobe ?좏샇瑜?'high'=> PORTB.2 = 1;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB, 1);
	// 16鍮꾪듃 ?곗씠?곗씤 cmd瑜?D0遺??1鍮꾪듃??吏곷젹濡??꾩넚?쒕떎.
	for( i = 0 ; i< 16 ; i++ )
	{
		// PORTB.0 = 1;
		if(cmd & 0x0001) HAL_GPIO_WritePin(GPIOB, LD1071_DIN,1);
		// PORTB.0 = 0;
		else HAL_GPIO_WritePin(GPIOB, LD1071_DIN,0);
		// PORTB.1 = 0;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
		// PORTB.1 = 1;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,1);
		// LSB遺???≪떊
		cmd = cmd >> 1 ;
	}
	// PORTB.1 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
}

void LD1071_Tx_Data(unsigned int data)
{
	unsigned int i;
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
	for( i =0 ; i < 16; i++ )
	{
		// PORTB.0 = 1;
		if( data & 0x0001) HAL_GPIO_WritePin(GPIOB, LD1071_DIN,1);
		// PORTB.0 = 0;
		else HAL_GPIO_WritePin(GPIOB, LD1071_DIN,0);
		// PORTB.1 = 0;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
		// PORTB.1 = 1;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,1);
		// LSB遺???≪떊
		data = data >> 1;
	}
	// PORTB.1 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
	// PORTB.2 = 1;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,1);
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
}

void LD1071_Global_Latch(void)
{
	unsigned int i = 0;
	// PORTB.2 = 1;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,1);
	// PORTB.0 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_DIN,0);
	for( i = 0 ; i < 9 ; i++ )
	{
		// PORTB.1 = 0;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
		// PORTB.1 = 1;
		HAL_GPIO_WritePin(GPIOB, LD1071_CLK,1);
	}
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
	// PORTB.2 = 1;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,1);
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
	// PORTB.2 = 1;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,1);
	// PORTB.2 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_STB,0);
	// PORTB.1 = 0;
	HAL_GPIO_WritePin(GPIOB, LD1071_CLK,0);
}

void LD1071_Tx_Display( unsigned int data_B, unsigned int data_C)
{
	unsigned int i , mask = 0x0001;
	LD1071_Tx_CMD( 0x0084);
	for(i = 0 ; i <16 ; i++)
	{
		if(data_B & mask ) LD1071_Tx_Data(0xFFFF);
		else LD1071_Tx_Data(0x0000);
		mask = mask << 1;
	}
	LD1071_Global_Latch();

	HAL_GPIO_WritePin(GPIOC, data_C,1);
	HAL_Delay(0);
	HAL_GPIO_WritePin(GPIOC, 0xff,0);

}

void LD1071_Reset(void)
{
	// Software Reset Command瑜??꾩넚??以??
	LD1071_Tx_CMD(0x0001);
	HAL_Delay(100);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  LD1071_Reset();
  HAL_GPIO_WritePin(GPIOB, LD1071_PWM, 1);
  HAL_GPIO_WritePin(GPIOC, 0xff,0);

  // Configuration Register Update Command
  LD1071_Tx_CMD(0x0034);
  // Configuration Register
  LD1071_Tx_Data(0x04FF);
  __flip();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	// Green
	  for(int i=0; i<2400; i++){	//異쒕젰 媛?닔??1ms???띾룄瑜?媛吏?
		  LD1071_Tx_Display(0xff00, 0xff);
	  	  }

	// RED
	  for(int i=0; i<2400; i++){	//異쒕젰 媛?닔??1ms???띾룄瑜?媛吏?
		  LD1071_Tx_Display(0x00ff, 0xff);
	  	  }

	//?뚰뙆踰?A
	  for(int i=0; i<300; i++){	//異쒕젰 媛?닔??1ms???띾룄瑜?媛吏? 300?대㈃ 300*(媛?닔)ms
	  LD1071_Tx_Display(0x0018, 0x01);	//??0011000, ??
	  LD1071_Tx_Display(0x0024, 0x02);	//??0100100, ??
	  LD1071_Tx_Display(0x0042, 0x04);	//??1000010, ??
	  LD1071_Tx_Display(0x0042, 0x08);	//??1000010, ??
	  LD1071_Tx_Display(0x007e, 0x10);	//??1111110, ??
	  LD1071_Tx_Display(0x0042, 0x20);	//??1000010, ??
	  LD1071_Tx_Display(0x0042, 0x40);	//??1000010, ??
	  LD1071_Tx_Display(0x0042, 0x80);	//??1000010, ??
	  }

	  for(int i=0; i<38; i++){	// 臾몄옄 ?곗씠???몄텧
		  for(int k=1; k<100; k++){	//異쒕젰 媛?닔??1ms???띾룄瑜?媛吏? 300?대㈃ 300*(媛?닔)ms
			  LD1071_Tx_Display(flipped_disp1ay[i][0], 0x01);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][1], 0x02);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][2], 0x04);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][3], 0x08);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][4], 0x10);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][5], 0x20);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][6], 0x40);	//??i, ??
			  LD1071_Tx_Display(flipped_disp1ay[i][7], 0x80);	//??i, ??
		  }
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 PC5 PC6 PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
