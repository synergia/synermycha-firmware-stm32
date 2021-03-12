/*
 * UART_DMA.h
 *
 *  Created on: 27.10.2019
 *      Author: Mateusz Salamon
 *		www.msalamon.pl
 *  Modified by: Sebastian Mróz
 *
 *      Website: https://msalamon.pl/odbieranie-uart-po-dma-to-bulka-z-maslem-lekcja-z-kursu-stm32/
 *      GitHub:  https://github.com/lamik/UART_DMA_receiving
 *      Contact: mateusz@msalamon.pl
 */

#include "stm32f4xx_hal.h"

#define DMA_RX_BUFFER_SIZE 64
#define UART_BUFFER_SIZE   256

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        UART_HandleTypeDef* huart;  // UART handler

        uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];  // DMA direct buffer
        uint8_t UART_Buffer[UART_BUFFER_SIZE];      // UART working circular buffer

        uint16_t UartBufferHead;
        uint16_t UartBufferTail;
        uint8_t UartBufferDataFrames;
    } UARTDMA_HandleTypeDef;

    void UARTDMA_UartIrqHandler(UARTDMA_HandleTypeDef* huartdma);
    void UARTDMA_DmaIrqHandler(UARTDMA_HandleTypeDef* huartdma);

    uint8_t UARTDMA_IsDataReady(UARTDMA_HandleTypeDef* huartdma);
    int UARTDMA_GetFrameFromBuffer(UARTDMA_HandleTypeDef* huartdma, char* OutBuffer);

    void UARTDMA_Init(UARTDMA_HandleTypeDef* huartdma, UART_HandleTypeDef* huart);

#ifdef __cplusplus
}
#endif