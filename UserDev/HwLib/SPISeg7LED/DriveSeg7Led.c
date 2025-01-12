/// @file DriveSeg7Led.h
/// @brief Seven segment LED drive function definition
#include "DriveSeg7Led.h"
#include "UartDebug.h"

tLedIndexDataSet Seg7LedIndexDataSet[] = {
  {ledIndex_invalid , 0x00}, // Invalid led index
  {ledIndex_id1     , 0x01}, // First led index from the right
  {ledIndex_id2     , 0x02},
  {ledIndex_id3     , 0x04},
  {ledIndex_id4     , 0x08},
  {ledIndex_id5     , 0x10},
  {ledIndex_id6     , 0x20},
  {ledIndex_id7     , 0x40},
  {ledIndex_id8     , 0x80},
};

tLedDigitDataSet Seg7LedDataSet[] = {
  {ledDigitInvalid  , 0xFF}, // Later now is silent
  {ledDigitZero     , 0xC0}, // 0
  {ledDigitOne      , 0xF9}, // 1
  {ledDigitTwo      , 0xA4}, // 2
  {ledDigitThree    , 0xB0}, // 3
  {ledDigitFour     , 0x99}, // 4
  {ledDigitFive     , 0x92}, // 5
  {ledDigitSix      , 0x82}, // 6
  {ledDigitSeven    , 0xF8}, // 7
  {ledDigitEight    , 0x80}, // 8
  {ledDigitNine     , 0x90}, // 9
  {ledDigitA        , 0x8C}, // A - Not test
  {ledDigitBlowcase , 0xBF}, // b - Not test
  {ledDigitC        , 0xC6}, // C - Not test
  {ledDigitDlowcase , 0xA1}, // d - Not test
  {ledDigitE        , 0x86}, // E - Not test
  {ledDigitF        , 0xFF}, // F - Not test
  {ledDigitMinus    , 0xBF}, // - - Not test
};

tLedIndex counterLedID_gdu8 = ledIndex_invalid;

tLedIndex getCurrentLedIndex(void) {
  return counterLedID_gdu8;
}

tCycleStatus setNextLedDisplayIndex(tLedIndex *nextLedIndex) {
  tCycleStatus retStatus = cycleNotComplete;
  // Check for valid ledIndex
  if ((counterLedID_gdu8 < ledIndex_id1) \
    ||(counterLedID_gdu8 > ledIndex_number)) {
    // Invalid led index
    uart_debug("Bug some where please check led id!!!\n");
    // Reset led id for recovery
    counterLedID_gdu8 = ledIndex_id1;
  } else {
  /* Continue work flow - increase counter index */
    counterLedID_gdu8++;
  }
  
  // Check if current led index reach limit led
  if (counterLedID_gdu8 == ledIndex_number) {
    // Reset counter index
    counterLedID_gdu8 = ledIndex_id1;
    // Completed one cycle
    retStatus = cycleCompleted;
  }
  
  // Set next led index display
  *nextLedIndex = counterLedID_gdu8;
  return retStatus;
}

void writeLED (tLedIndex index, tLedDigit digit) {
  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi2, &Seg7LedDataSet[digit].ledData, 1, 100);
  HAL_SPI_Transmit(&hspi2, &Seg7LedIndexDataSet[index].ledIndexData, 1, 100);
  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
}

#define DP_BIT_MASK         0x80

void writeLedWithPoint (tLedIndex index, tLedDigit digit) {
  uint8_t Seg7LedDataUpdated = Seg7LedDataSet[digit].ledData;
  // Update point at led index 7 and 2
  if ((index == ledIndex_id7) || (index == ledIndex_id2)) {
    Seg7LedDataUpdated &= ~(DP_BIT_MASK);
  }
  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi2, &Seg7LedDataUpdated, 1, 100);
  HAL_SPI_Transmit(&hspi2, &Seg7LedIndexDataSet[index].ledIndexData, 1, 100);
  HAL_GPIO_WritePin(SEG7LED_SPI2_CS_GPIO_Port, SEG7LED_SPI2_CS_Pin, GPIO_PIN_SET);
}

void setLedDisplayAll(tLedDigit digit) {
  tLedIndex ledDisplayIndex;
  // Decide led index to be displayed
  (void)setNextLedDisplayIndex(&ledDisplayIndex);
  // Export data to SPI line for display
  writeLED(ledDisplayIndex, digit);
}

tLedDigit displayBuffer_xda8[8];
#define INVALID_DIGIT       99

void extractData2Buffer(uint16_t firstData, uint16_t secondData) {
  // Get hundred digit
  uint16_t currDigit, remainData;
  
  // Check for input argument valid
  if (firstData > 999 || secondData > 999) {
    for (int i = 0; i < 8; i++) {
      displayBuffer_xda8[i] = ledDigitInvalid;
    }
  }
  
  // Extract first data temperature
  remainData = firstData;
  /* Fist digit */
  currDigit = remainData/100;
  remainData = remainData - (currDigit*100);
  displayBuffer_xda8[7] = (tLedDigit)(currDigit + 1);
  /* Second digit */
  currDigit = remainData/10;
  remainData = remainData - (currDigit*10);
  displayBuffer_xda8[6] = (tLedDigit)(currDigit + 1);
  /* Third digit */
  currDigit = remainData;
  remainData = remainData - (currDigit);
  displayBuffer_xda8[5] = (tLedDigit)(currDigit + 1);
  displayBuffer_xda8[4] = ledDigitMinus;
  
  // Extract second data humidity
  displayBuffer_xda8[3] = ledDigitMinus;
  remainData = secondData;
  /* Fist digit */
  currDigit = remainData/100;
  remainData = remainData - (currDigit*100);
  displayBuffer_xda8[2] = (tLedDigit)(currDigit + 1);
  /* Second digit */
  currDigit = remainData/10;
  remainData = remainData - (currDigit*10);
  displayBuffer_xda8[1] = (tLedDigit)(currDigit + 1);
  /* Third digit */
  currDigit = remainData;
  remainData = remainData - (currDigit);
  displayBuffer_xda8[0] = (tLedDigit)(currDigit + 1);
}


void setLedDisplayPeriod(uint16_t firstData, uint16_t secondData) {
  // One cycle, 8 led will be updated
  tLedIndex ledDisplayIndex;
  
  if (cycleCompleted == setNextLedDisplayIndex(&ledDisplayIndex)) {
    // When one cycle completed, next 8 data display buffer will be filled
    extractData2Buffer(firstData, secondData);
  } else { /* Old data will be displayed */ }
  
  // Write data to led, mapped bw index and buffer
  // Check for safe case
  if ((ledDisplayIndex - 1) > 7) {
    uart_debug("Bug index!!!!! n");
    return;
  }
  writeLedWithPoint(ledDisplayIndex, displayBuffer_xda8[ledDisplayIndex-1]);
}


uint16_t firstLedData_gdu16;
uint16_t secondLedData_gdu16;

void ledDisplayUpdateData(uint16_t firstData, uint16_t secondData) {
  firstLedData_gdu16 = firstData;
  secondLedData_gdu16 = secondData;
}

void ledDisplayUpdatePeriod(void) {
  setLedDisplayPeriod(firstLedData_gdu16, secondLedData_gdu16);
}

