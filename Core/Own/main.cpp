#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

void SystemClock_Config(void);
extern DMA_HandleTypeDef hdma_usart1_rx;


#ifdef __cplusplus
}
#endif
#include "pict.h"
#include "LCD.h"

uint8_t pitc[LCD_W* LCD_H * 2] = {0};

LCD lcd(GPIOC,NULL,NULL,GPIO_PIN_11,GPIO_PIN_12,NULL,GPIO_PIN_10,&hspi1);
int num = 0;
int max = 0;
extern int sec;
extern uint8_t one_frame_done;
extern bool val;
int main(void)
{

    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_SPI1_Init();
    MX_TIM2_Init();
    MX_USART1_UART_Init();

    HAL_TIM_Base_Start_IT(&htim2);



    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)lcd.buffer, LCD_W * LCD_H * 2);
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);


    lcd.Init();

    lcd.Fill(0, 0, LCD_W, LCD_H, WHITE);


    lcd.DMA_Fill();
    int n=0;
    while (1){

//        if (num<20){
//            lcd.Fill(0, 0, LCD_W, LCD_H, WHITE);
//        } else{
//            lcd.Fill(0, 0, LCD_W, LCD_H, GREEN);
//        }
        ++num;
        lcd.ShowIntNum(20, 140, max, 4, RED, WHITE, 16);
        lcd.DMA_Fill();
        while (one_frame_done);
        one_frame_done = true;

        if (val) {

//            lcd.ShowPicture(0,0,LCD_H,LCD_W,pitc);
//            HAL_UART_Transmit(&huart1,(uint8_t*)"g",4,0);
            lcd.ShowPicture(0,0,LCD_H,LCD_W,pitc);

            val=0;
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)pitc, LCD_W * LCD_H * 2);
            __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
        }



    }
}















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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
