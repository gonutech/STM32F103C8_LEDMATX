/*
 * MAX7219.c
 *
 *  Created on: 16 Jan 2023
 *      Author: EDISON NGUNJIRI
 */
#include "MAX7219.h"
#include "UartDebug.h"


#define CHAR_MAX 38

uint8_t decode[2];

uint8_t data[2];
uint8_t shutdown[2];

uint8_t i=0;
uint8_t ON=0xFF;
uint8_t OFF=0x00;

const uint8_t InitCommands[10][2]={
		{0x09,0x00}, // Set no decode mode
		{0x0A,0x0C}, // Set intensity to some value, min = 0, max = 0x0F
		{0x0B,0x07}, // Set the scan limit to display digits 0,1,2,3,4,5,6,7
		{0x0C,0x01}, // Set normal operation mode in shutdown register
		{0x0F,0x00}  // Set display test register format to normal operation
};
uint8_t LETTERS[CHAR_MAX][8]={
		{0x3c,0x24,0x24,0x24,0x24,0x3c,0x00,0x00},/*0*/
		{0x08,0x18,0x28,0x08,0x08,0x3c,0x00,0x00},/*1*/
		{0x3c,0x04,0x3c,0x20,0x20,0x3c,0x00,0x00},/*2*/
		{0x3c,0x04,0x3c,0x04,0x04,0x3c,0x00,0x00},/*3*/
    {0x20,0x20,0x28,0x3c,0x08,0x08,0x00,0x00},/*4*/
		{0x00,0x1C,0x20,0x24,0x1C,0x04,0x3C,0x00},/*5*/
		{0x00,0x1C,0x24,0x24,0x1C,0x04,0x38,0x00},/*6*/
		{0x00,0x10,0x10,0x38,0x10,0x20,0x3C,0x00},/*7*/
		{0x00,0x18,0x24,0x24,0x18,0x24,0x18,0x00},/*8*/
		{0x00,0x04,0x08,0x18,0x24,0x24,0x18,0x00},/*9*/
		{0x00,0x24,0x24,0x3C,0x24,0x24,0x18,0x00},/*A*/
		{0x00,0x1C,0x24,0x24,0x1C,0x24,0x1C,0x00},/*B*/
		{0x00,0x38,0x04,0x04,0x04,0x04,0x38,0x00},/*C*/
		{0x00,0x1C,0x24,0x24,0x24,0x24,0x1C,0x00},/*D*/
		{0x00,0x3C,0x04,0x3C,0x04,0x3C,0x00,0x00},/*E*/
		{0x00,0x04,0x04,0x1C,0x04,0x04,0x3C,0x00},/*F*/
		{0x00,0x38,0x24,0x34,0x04,0x24,0x18,0x00},/*G*/
		{0x00,0x24,0x24,0x3C,0x24,0x24,0x00,0x00},/*H*/
		{0x00,0x1C,0x08,0x08,0x08,0x1C,0x00,0x00},/*I*/
		{0x00,0x1C,0x10,0x10,0x10,0x38,0x00,0x00},/*J*/
		{0x00,0x24,0x14,0x0C,0x14,0x24,0x04,0x00},/*K*/
		{0x00,0x3C,0x04,0x04,0x04,0x04,0x04,0x00},/*L*/
		{0x00,0x22,0x22,0x2A,0x36,0x22,0x00,0x00},/*M*/
		{0x00,0x22,0x32,0x2A,0x26,0x22,0x22,0x00},/*N*/
		{0x00,0x18,0x24,0x24,0x24,0x24,0x18,0x00},/*O*/
		{0x00,0x04,0x04,0x1C,0x24,0x24,0x1C,0x00},/*P*/
		{0x40,0x38,0x34,0x24,0x24,0x24,0x18,0x00},/*Q*/
		{0x00,0x24,0x14,0x1C,0x24,0x24,0x1C,0x00},/*R*/
		{0x18,0x24,0x20,0x18,0x04,0x24,0x18,0x00},/*S*/
		{0x00,0x08,0x08,0x08,0x08,0x08,0x3E,0x00},/*T*/
		{0x00,0x18,0x24,0x24,0x24,0x24,0x00,0x00},/*U*/
		{0x00,0x08,0x14,0x22,0x22,0x22,0x22,0x00},/*V*/
		{0x00,0x44,0x6C,0x54,0x44,0x44,0x00,0x00},/*W*/
		{0x00,0x28,0x28,0x10,0x28,0x28,0X00,0x00},/*X*/
		{0x10,0x10,0x10,0x18,0x24,0x24,0x24,0x00},/*Y*/
		{0x00,0x7C,0x08,0x10,0x20,0x7C,0x00,0x00},/*Z*/
		{0x00,0x18,0x3C,0x7E,0x7F,0x77,0x23,0x00},/*HEART ON*/
		{0xFF,0xE7,0xC3,0xC1,0x81,0x81,0xCB,0xFF},/*HEART OFF*/
};


#define ROWMAX 9
uint8_t CHECKROW[ROWMAX][8]={
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

void ledMatI_ClearDisplay(void) {
  for (int address = 1; address <= 8; address++) {
    // Since digit 0 address is 1, start of for loop is 1 and end at 8
    shutdown[0] = address;
    shutdown[1] = 0x0; // Data bit set to 0 mean turn it off
    HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
  }
}

void MAX7219_Init(void)
{
  // Control register set
  for(i=0;i<5;i++)
  {
    shutdown[0]=InitCommands[i][0];
    shutdown[1]=InitCommands[i][1];
    HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, shutdown, 2, 1000);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
  }
  // Clear display
  // ledMatI_ClearDisplay();
}

void Print_Alphabet(uint8_t i)
{
		for(int k=1;k<=8;k++)
		{
			shutdown[0]=k;
			shutdown[1]=LETTERS[i-1][k-1];
		  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
		  HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
  		HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
		}
}

void Print_RowCheck(uint8_t i)
{
//		for(int k=1;k<=8;k++)
//		{
//			shutdown[0]=k;
//			shutdown[1]=CHECKROW[i-1][k-1];
//		  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
//		  HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
//  		HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
//		}

  for (int k = 1; k <= 8; k++) {
      shutdown[0]=k-1;
      shutdown[1]=CHECKROW[i][k-1];
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
  }
}

void ledMatI_SetSingleRowDisplay(uint8_t RowNo, uint8_t RowData) {
    uint8_t InvalidCommandBuff[13] = "ColNotValid\n";
    // Since digit 0 address is 1, start of for loop is 1 and end at 8
    // Using only 6 colum, let restricted to 6
    if ( (RowNo == 0)|| (RowNo > 6)) {
      uartDebugI_PrintDebugInfo(InvalidCommandBuff, 13);
    } else {
      shutdown[0] = RowNo;
      shutdown[1] = RowData; // Data bit set to 0 mean turn it off
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
    }
}


uint8_t SingleLed[8] = { 0x80,0x40,0x20,0x10, 0x08,0x04,0x02,0x01}; 
void ledMatI_SetSingleRowSingleLedDisplay(uint8_t RowNo, uint8_t SingleLedId) {
    uint8_t InvalidCommandBuff[13] = "ColNotValid\n";
    // Since digit 0 address is 1, start of for loop is 1 and end at 8
    // Using only 6 colum, let restricted to 6
    if ( (RowNo == 0)||(RowNo > 6)||(SingleLedId>7)) {
      uartDebugI_PrintDebugInfo(InvalidCommandBuff, 13);
    } else {
      shutdown[0] = RowNo;
      shutdown[1] = SingleLed[SingleLedId]; // Data bit set to 0 mean turn it off
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
    }
}

uint8_t IncreaseLed[8] = { 0x80,0xC0,0xE0,0xF0, 0xF8,0xFC,0xFE,0xFF};
void ledMatI_SetSingleRowIncreaseLedDisplay(uint8_t RowNo, uint8_t IncreaseLedId) {
    uint8_t InvalidCommandBuff[13] = "ColNotValid\n";
    // Since digit 0 address is 1, start of for loop is 1 and end at 8
    // Using only 6 colum, let restricted to 6
    if ( (RowNo == 0)||(RowNo > 6)||(IncreaseLedId>7)) {
      uartDebugI_PrintDebugInfo(InvalidCommandBuff, 13);
    } else {
      shutdown[0] = RowNo;
      shutdown[1] = IncreaseLed[IncreaseLedId]; // Data bit set to 0 mean turn it off
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
    }
}

uint8_t IncreaseLedInv[8] = { 0x01,0x03,0x07,0x0F, 0x1F,0x3F,0x7F,0xFF};
void ledMatI_SetSingleRowIncreaseInvLedDisplay(uint8_t RowNo, uint8_t IncreaseLedId) {
    uint8_t InvalidCommandBuff[13] = "ColNotValid\n";
    // Since digit 0 address is 1, start of for loop is 1 and end at 8
    // Using only 6 colum, let restricted to 6
    if ( (RowNo == 0)||(RowNo > 6)||(IncreaseLedId>7)) {
      uartDebugI_PrintDebugInfo(InvalidCommandBuff, 13);
    } else {
      shutdown[0] = RowNo;
      shutdown[1] = IncreaseLedInv[IncreaseLedId]; // Data bit set to 0 mean turn it off
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(&hspi2, shutdown, 2, HAL_MAX_DELAY);
      HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
    }
}


