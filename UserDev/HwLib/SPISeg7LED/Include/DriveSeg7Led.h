/// @file DriveSeg7Led.h
/// @brief Seven segment LED drive function declaration

#ifndef __DRIVESEG7LED_H__
#define __DRIVESEG7LED_H__

#include "stdint.h"
// External HAL driver include
#include "spi.h"

extern SPI_HandleTypeDef hspi2;

typedef enum {
  ledDigitInvalid = 0,
  ledDigitZero,
  ledDigitOne,
  ledDigitTwo,
  ledDigitThree,
  ledDigitFour,
  ledDigitFive,
  ledDigitSix,
  ledDigitSeven,
  ledDigitEight,
  ledDigitNine,
  ledDigitA,
  ledDigitBlowcase,
  ledDigitC,
  ledDigitDlowcase,
  ledDigitE,
  ledDigitF,
  ledDigitMinus,
  ledDigitNumber
} tLedDigit;

typedef enum {
  cycleNotComplete = 0,
  cycleCompleted,
} tCycleStatus;

typedef struct {
  tLedDigit ledID;
  uint8_t ledData;
} tLedDigitDataSet; /* Led digit display matrix */

typedef enum {
  ledIndex_invalid = 0,
  ledIndex_id1,
  ledIndex_id2,
  ledIndex_id3,
  ledIndex_id4,
  ledIndex_id5,
  ledIndex_id6,
  ledIndex_id7,
  ledIndex_id8,
  ledIndex_number
} tLedIndex; /* Led index count from the right */

typedef struct {
  tLedIndex ledIndex;
  uint8_t ledIndexData;
} tLedIndexDataSet; /* Led index for scanner */



extern void setLedDisplayAll(tLedDigit digit);
extern void setLedDisplayPeriod(uint16_t firstData, uint16_t secondData);
extern void ledDisplayUpdatePeriod(void);
extern void ledDisplayUpdateData(uint16_t firstData, uint16_t secondData);

#endif /* __DRIVESEG7LED_H__ */
