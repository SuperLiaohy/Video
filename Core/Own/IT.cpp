//
// Created by lhy on 5/18/24.
//
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "spi.h"
#include "usart.h"

extern DMA_HandleTypeDef hdma_usart1_rx;

#ifdef __cplusplus
}
#endif

#include "LCD.h"
#include "uart.h"
extern LCD lcd;
extern int num;
extern int max;

//uart u1(&huart1, &hdma_usart1_rx, (uint8_t*)lcd.buffer, LCD_W * LCD_H * 2);
int sec = 0;
int z;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim==&htim2){
        ++sec;
        max = num;
        num = 0;
    }
}

volatile bool one_frame_done = true;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi1) { one_frame_done = false; }
}

volatile bool val = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
//    u1.print("%d\n", Size);

    val = 1;



//    if (Size == LCD_W * LCD_H * 2) {

//    }

}