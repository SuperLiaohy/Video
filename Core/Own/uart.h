//
// Created by lhy on 5/21/24.
//

#ifndef SPI_DMA_UART_H
#define SPI_DMA_UART_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#ifdef __cplusplus
}
#endif

class uart {
private:
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdma;
    uint8_t *buf;
    uint16_t size;
public:
    uart(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma, uint8_t *buf, uint16_t size) {
        this->huart = huart;
        this->hdma = hdma;
        this->buf = buf;
        this->size = size;
    }

    void start() {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, buf, size);
        __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
    }

    void callback() {
        HAL_UARTEx_ReceiveToIdle_DMA(huart, buf, size);
        __HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
    }

    void print(const char * buf, ...)
    {
        const char *p = (char *)buf;
        char str[255] = {0};
        va_list v;
        va_start(v, (char *)buf);
        vsprintf(str, (char *)buf, v); //使用可变参数的字符串打印。类似sprintf
        HAL_UART_Transmit(huart, (uint8_t*)str, strlen(str), 0xff);
        va_end(v);
    }
};


#endif //SPI_DMA_UART_H
