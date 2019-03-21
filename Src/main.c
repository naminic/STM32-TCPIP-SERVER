/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcp.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYDEBOUNCE  	20
#define MAXDATALEN 		10	
#define LED1_ON  			'1'
#define LED2_ON				'2'		
#define LED3_ON				'3'
#define LED4_ON				'4'
#define LED1_OFF			'5'
#define LED2_OFF			'6'
#define LED3_OFF			'7'
#define LED4_OFF			'8'
#define GET_PB				'9'	
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void server_init				(u16_t port);
void 	server_conn_err		(void *arg, err_t err);
err_t server_recv				(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
err_t server_accept			(void *arg, struct tcp_pcb *pcb, err_t err);
char 	get_key_state			(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern __IO uint32_t uwTick;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_LWIP_Init();
  /* USER CODE BEGIN 2 */
  server_init(1000);
	
  
  /* Notify user about the network interface config */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			MX_LWIP_Process();

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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED3_Pin|LED4_Pin|LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED3_Pin LED4_Pin LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin|LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1_Pin PB2_Pin PB3_Pin */
  GPIO_InitStruct.Pin = PB1_Pin|PB2_Pin|PB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4_Pin */
  GPIO_InitStruct.Pin = PB4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PB4_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


/**
  * @brief  Called when a data is received on the server connection
  * @param  arg	the user argument
  * @param  pcb	the tcp_pcb that has received the data
  * @param  p	the packet buffer
  * @param  err	the error value linked with the received data
  * @retval error value
  */
err_t server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{

	
  char *c;
  int i;
	char data_read[MAXDATALEN]={0};
	char data_send[MAXDATALEN]={0};
 

  /* We perform here any necessary processing on the pbuf */
  if (p != NULL) 
  {        
	/* We call this function to tell the LwIp that we have processed the data */
	/* This lets the stack advertise a larger window, so more data can be received*/
	tcp_recved(pcb, p->tot_len);
		
	c = p->payload;	
		
	for( i = 0 ; i < p->len ; i++)
  {
	  data_read[i] = c[i];
	}		
	
	switch (data_read[0])
	{
	  case LED1_ON:
			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,1);
			break;
		case LED2_ON:
			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,1);
			break;
		case LED3_ON:
			HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,1);
			break;
		case LED4_ON:
			HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,1);
			break;
		case LED1_OFF:
			HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,0);
			break;
		case LED2_OFF:
			HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,0);
			break;
		case LED3_OFF:
			HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,0);
			break;
		case LED4_OFF:
			HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin,0);
			break;
		case GET_PB:
			sprintf(data_send,"%d",get_key_state());
		  tcp_write(pcb,data_send,strlen(data_send),1);
			break;
	}


	
	/* End of processing, we free the pbuf */
    pbuf_free(p);
  }  
  else if (err == ERR_OK) 
  {
    /* When the pbuf is NULL and the err is ERR_OK, the remote end is closing the connection. */
    /* We close the connection */
    return tcp_close(pcb);
  }
  return ERR_OK;


}

/**
  * @brief  This function when the server connection is established
  * @param  arg  user supplied argument 
  * @param  pcb	 the tcp_pcb which accepted the connection
  * @param  err	 error value
  * @retval ERR_OK
  */
err_t server_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{     
  /* Configure LwIP to use our call back functions. */
  tcp_err(pcb, server_conn_err);
  tcp_recv(pcb, server_recv);
  return ERR_OK;
}

/**
  * @brief  Initialize the server application  
  * @param  None 
  * @retval None 
  */
void server_init(u16_t port)
{
  struct tcp_pcb *pcb;	            		
  
  /* Create a new TCP control block  */
  pcb = tcp_new();	                		 	

  /* Assign to the new pcb a local IP address and a port number */
  /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
  tcp_bind(pcb, IP_ADDR_ANY, port);       


  /* Set the connection to the LISTEN state */
  pcb = tcp_listen(pcb);				

  /* Specify the function to be called when a connection is established */	
  tcp_accept(pcb, server_accept);   
										
}

/**
  * @brief  This function is called when an error occurs on the connection 
  * @param  arg
  * @parm   err
  * @retval None 
  */
void server_conn_err(void *arg, err_t err)
{
  struct name *name;
  name = (struct name *)arg;

  mem_free(name);
}


char get_key_state(void)
{
  if(!HAL_GPIO_ReadPin(PB1_GPIO_Port,PB1_Pin))
	{
		HAL_Delay(KEYDEBOUNCE);
		if(!HAL_GPIO_ReadPin(PB1_GPIO_Port,PB1_Pin))
		{	
		 while(!HAL_GPIO_ReadPin(PB1_GPIO_Port,PB1_Pin)); 	
		 return 1;
		}			
	}	
	
  if(!HAL_GPIO_ReadPin(PB2_GPIO_Port,PB2_Pin))
	{
		HAL_Delay(KEYDEBOUNCE);
		if(!HAL_GPIO_ReadPin(PB2_GPIO_Port,PB2_Pin))
		{	
		 while(!HAL_GPIO_ReadPin(PB2_GPIO_Port,PB2_Pin)); 	
		 return 2;
		}	
	}

  if(!HAL_GPIO_ReadPin(PB3_GPIO_Port,PB3_Pin))
	{
		HAL_Delay(KEYDEBOUNCE);
		if(!HAL_GPIO_ReadPin(PB3_GPIO_Port,PB3_Pin))
		{	
		 while(!HAL_GPIO_ReadPin(PB3_GPIO_Port,PB3_Pin)); 	
		 return 3;
		}		
	}

  if(!HAL_GPIO_ReadPin(PB4_GPIO_Port,PB4_Pin))
	{
		HAL_Delay(KEYDEBOUNCE);
		if(!HAL_GPIO_ReadPin(PB4_GPIO_Port,PB4_Pin))
		{	
		 while(!HAL_GPIO_ReadPin(PB4_GPIO_Port,PB4_Pin)); 	
		 return 4;
		}	
	}

  return 0;	
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
