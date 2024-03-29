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
  *Paweekorn
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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[20];
uint8_t TxBuffer[40];
uint8_t data[5];
uint8_t Mode = 0;
float freq = 10.0;
int i_freq = 10;
uint8_t flag = 1;
uint8_t status;
uint8_t length;

char int_str[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void UARTDMAConfig();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t text[] = "SELECT MENU [0(LED),1(BUTT)]\r\n"; //initial
  HAL_UART_Transmit(&huart2, text,32, 1000); //polling

  UARTDMAConfig();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  DummyTask();
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 500000;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2){
		//(for string only) Add string stop symbol \0 to end string
		RxBuffer[1] = '\0';
		switch(Mode){
		case 0:
			if(RxBuffer[0] == '0')
			{
				sprintf((char*)TxBuffer,"LED[a(+),s(-),d(O/C),x(back)] : %s\r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));

				Mode = 1;
			}
			else if(RxBuffer[0] == '1'){

				sprintf((char*)TxBuffer,"BUTT[x (Back)] : %s\r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));

				Mode = 2;
			}

			else{
				sprintf((char*)TxBuffer,"SELECT MENU [0(LED),1(BUTT)] : %s\r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			break;

		case 1: //LED (0)
			if(RxBuffer[0] == 'a')
			{
				freq += 1;
				i_freq = freq;
				sprintf((char*)TxBuffer,"Frequency : %d\r\n",i_freq);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			else if(RxBuffer[0] == 's'){
				if(freq > 1){
					freq -= 1;
				}
				i_freq = freq;
				sprintf((char*)TxBuffer,"Frequency : %d\r\n",i_freq);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			else if(RxBuffer[0] == 'd'){
				if(flag == 0){
					flag = 1;
					sprintf((char*)TxBuffer,"LED ON : %u\r\n",flag);
				}
				else{
					flag = 0;
					sprintf((char*)TxBuffer,"LED OFF : %u\r\n",flag);
				}
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			else if(RxBuffer[0] == 'x'){
				sprintf((char*)TxBuffer,"SELECT MENU [0(LED),1(BUTT)] : \r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				Mode = 0;
			}
			else{
				sprintf((char*)TxBuffer,"LED[a(+),s(-),d(O/C),x(back)] : %s\r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			break;

		case 2: //BUTT (1)
			if(RxBuffer[0] == 'x'){
				sprintf((char*)TxBuffer,"SELECT MENU [0(LED),1(BUTT)] : \r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				Mode = 0;
			}
			else{
				sprintf((char*)TxBuffer,"BUTTON [x(back)] : %s\r\n",RxBuffer);
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			break;
		}
	}
}

void UARTDMAConfig(){
	//start UART in DMA Mode
	HAL_UART_Receive_DMA(&huart2, RxBuffer, 1);
}

void DummyTask()
{
	static uint32_t timestamp = 0;
	if(HAL_GetTick() >= timestamp){
		timestamp = HAL_GetTick() + (1/freq)*1000;
		if(flag != 0){
			HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		}
		else if(flag == 0){
			HAL_GPIO_WritePin(LD2_GPIO_Port,LD2_Pin,0);
		}

		if (Mode == 2){
			status = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
			if(status == 1){
				sprintf(int_str, "ON\r\n", status);
				HAL_UART_Transmit(&huart2,int_str,5, 10);
			}
			else if(status == 0){
				sprintf(int_str, "OFF\r\n", status);
				HAL_UART_Transmit(&huart2,int_str,6, 10);
			}
		}
	}
}
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
