/*
 * uart.c
 *
 *  Created on: Jan 12, 2024
 *      Author: gdh13
 */

/*
 * uart.c
 *
 *  Created on: Feb 3, 2023
 *      Author: pusedu03
 */

#include "uart.h"
#include <stdio.h>

UART_HandleTypeDef *huart;
//ring buffer
#define rxBufferMax	255
int rxBufferGp;					// get pointer (read)
int rxBufferPp;					// put pointer (write)
uint8_t rxBuffer[rxBufferMax];
uint8_t rxChar; //for interrupt

// uart 장치 초기화
void initUart(UART_HandleTypeDef *inHuart) {
	huart = inHuart;
	HAL_UART_Receive_IT(huart, &rxChar, 1); //receive interrupt
}

// 문자수신 처리 - 실제로 인터럽트 발생되면 점프해 들어오는 곳
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	rxBuffer[rxBufferPp++] = rxChar;
	rxBufferPp %= rxBufferMax; //for 255->0
	HAL_UART_Receive_IT(huart, &rxChar, 1); //interrupt end->re tjfwjd
}

// 버퍼에서 문자 꺼내오기
uint8_t getChar() {
	uint8_t result;
	if(rxBufferGp == rxBufferPp) return 0; // no read content for ascii cuz return 0(zero)
	result = rxBuffer[rxBufferGp++];
	rxBufferGp %= rxBufferMax;
	return result;
}

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(huart, p, len, len);
	return len;
}
