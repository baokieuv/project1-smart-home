#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "led_fan.h"
#include "door.h"
#include "other_devices.h"

#define NUMBER_OF_LED_FAN	8
#define NUMBER_OF_DOOR		2
#define NUMBER_OF_DEVICES	7

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

uint8_t size = 0;

Led_Fan array_led_fan[NUMBER_OF_LED_FAN] = {
		{1, 0, TIM2, TIM_CHANNEL_1, GPIOA, GPIO_PIN_0},
		{2, 0, TIM2, TIM_CHANNEL_2, GPIOA, GPIO_PIN_1},
		{7, 0, TIM3, TIM_CHANNEL_1, GPIOA, GPIO_PIN_6},
		{10, 0, TIM3, TIM_CHANNEL_2, GPIOA, GPIO_PIN_7},
		{11, 0, TIM3, TIM_CHANNEL_3, GPIOB, GPIO_PIN_0},
		{13, 0, TIM3, TIM_CHANNEL_4, GPIOB, GPIO_PIN_1},
		{14, 0, TIM1, TIM_CHANNEL_1, GPIOA, GPIO_PIN_8},
		{16, 0, TIM1, TIM_CHANNEL_4, GPIOA, GPIO_PIN_11},
};

Door array_door[NUMBER_OF_DOOR] = {
		{5, 0, TIM2, TIM_CHANNEL_3, GPIOA, GPIO_PIN_2},
		{6, 0, TIM2, TIM_CHANNEL_4, GPIOA, GPIO_PIN_3},
};

Oher_Devices array_devices[NUMBER_OF_DEVICES] = {
		{3, 0, GPIOA, GPIO_PIN_4},
		{4, 0, GPIOA, GPIO_PIN_5},
		{8, 0, GPIOB, GPIO_PIN_3},
		{9, 0, GPIOB, GPIO_PIN_4},
		{12, 0, GPIOB, GPIO_PIN_5},
		{15, 0, GPIOB, GPIO_PIN_12},
		{17, 0, GPIOB, GPIO_PIN_13},
};


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);

int main(void)
{


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK){
	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4) != HAL_OK){
	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3) != HAL_OK){
  	  Error_Handler();
  }
  if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4) != HAL_OK){
  	  Error_Handler();
  }

  HAL_UART_Receive_IT(&huart1, &size, 1);
  /* USER CODE END 2 */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, ENABLE);
  /* Infinite loop */
  while (1)
  {

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 719;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 719;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.Pulse = 50;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 719;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
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
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB3 PB4
                           PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void control_device(char *detail[]){
	uint8_t flag = 0;

	if(strcmp(detail[0], "LED_FAN") == 0){
		Led_Fan *device = NULL;
		for(int i = 0; i < NUMBER_OF_LED_FAN; i++){
			if(array_led_fan[i].id == atoi(detail[1])){
				device = &array_led_fan[i];
				flag = 1;
				break;
			}
		}
		if(flag){
			device->brightness = atoi(detail[2])*15;
			Control_LED_FAN(device);
		}
	}
	else if(strcmp(detail[0], "DOOR") == 0){
		Door *door = NULL;
		for(int i = 0; i < NUMBER_OF_DOOR; i++){
			if(array_door[i].id == atoi(detail[1])){
				door = &array_door[i];
				flag = 1;
				break;
			}
		}
		if(flag){
			door->status = strcmp(detail[2], "OPEN") == 0;
			Control_DOOR(door);
		}
	}
	else if(strcmp(detail[0], "DEVICES") == 0){
		Oher_Devices *device = NULL;
		for(int i = 0; i < NUMBER_OF_DEVICES; i++){
			if(array_devices[i].id == atoi(detail[1])){
				device = &array_devices[i];
				flag = 1;
				break;
			}
		}
		if(flag){
			device->status = strcmp(detail[2], "ON") == 0;
			Control_Device(device);
		}
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		char *data = (char*) malloc(100);
		uint8_t s = size;
		HAL_UART_Receive(huart, (uint8_t*)data, size, HAL_MAX_DELAY);
		data[s] = '\0';
	    char *token = strtok(data, "+");
	    char *detail[4];
	    for (int j = 0; j < 4; j++) {
	        detail[j] = malloc(20); // Giả sử chuỗi có thể chứa tối đa 100 ký tự
	    }

	    char *val = strtok(NULL, "+");
	    int i = 0;
	    while(val != NULL && i < 4){
	    	strcpy(detail[i++], val);
	    	val = strtok(NULL, "+");
	    }

	    if(strcmp(token, "CONTROL") == 0){
	    	control_device(detail);
	    }
	    else if(strcmp(token, "FIRST_INIT") == 0){
	    	char mess[200] = "";
	    	for(int i = 0; i < NUMBER_OF_LED_FAN; i++){
	    		char id[4], brightness[6];
	    		sprintf(id, "%d", array_led_fan[i].id);
	    		sprintf(brightness, "%d", array_led_fan[i].brightness);

	    		strcat(mess, id);
	    		strcat(mess, "+");
	    		strcat(mess, brightness);
	    		strcat(mess, " ");
	    	}
	    	for(int i = 0; i < NUMBER_OF_DOOR; i++){
	    		char id[4];
	    		sprintf(id, "%d", array_door[i].id);

				strcat(mess, id);
				strcat(mess, "+");
				strcat(mess, array_door[i].status == 0 ? "CLOSE" : "OPEN");
				strcat(mess, " ");
			}
	    	for(int i = 0; i < NUMBER_OF_DEVICES; i++){
	    		char id[4];
	    		sprintf(id, "%d", array_devices[i].id);

				strcat(mess, id);
				strcat(mess, "+");
				strcat(mess, array_devices[i].status == 0 ? "OFF" : "ON");
				strcat(mess, " ");
			}

	    	uint8_t len = strlen(mess);
	    	HAL_UART_Transmit(&huart1, &len, 1, HAL_MAX_DELAY);
	    	HAL_Delay(10);
	    	HAL_UART_Transmit(&huart1, (uint8_t*)mess, len, HAL_MAX_DELAY);
	    }
		free(data);
		for (int j = 0; j < 4; j++) {
		    free(detail[j]);
		}
		HAL_UART_Receive_IT(huart, &size, 1);
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
