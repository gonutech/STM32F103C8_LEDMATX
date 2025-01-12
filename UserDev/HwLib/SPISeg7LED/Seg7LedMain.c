/// @file Seg7LedMain.c
/// @brief Seven segmant LED main function definition

#include "Seg7Led.h"
#include "MAX7219.h"
#include "UartDebug.h"

typedef enum  {
  P1_CLEAR_DISPLAY = 0,
  P2_SNAKE_AFFECT,
  P3_NUMCOUNT,
  P4_RANDOMNUMBER,
  P5_KEEPSINGLECOL,
  P6_IDLERUN,
} LEDMATPROCESSID_t;
typedef enum {
  LEDMATPROC_READY = 0,
  LEDMATPROC_ONGOING,
  LEDMATPROC_FINISHED
} LEDMATPROCSTATE_t;

typedef struct {
  LEDMATPROCESSID_t LedMatProcID;
  LEDMATPROCSTATE_t LedMatProcState; // 0 Mean ready - 1 Mean executing - 2 Mean finished
} STATEAFFECT_t;

STATEAFFECT_t ledMatProcessInstance;

#define DBSTATEBUFSIZE     11
#define DBSTATEBUFNUM      9
uint8_t DebugPrintState[DBSTATEBUFSIZE] = "CurrStateX\n";

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
void ledMatI_LedScannerInit(void) {
  /* To do */
  static uint8_t dbInitCompleteBuff[] = "LedInitFinished\n";

  MAX7219_Init();

  ledMatProcessInstance.LedMatProcID    = P1_CLEAR_DISPLAY;
  ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
  uartDebugI_PrintDebugInfo(dbInitCompleteBuff, 16);
}

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
uint16_t ledMatI_LedScannerTask(void) {
  /* This task will do the process control */
  uint8_t CurrentProcInstance = ledMatProcessInstance.LedMatProcID;
  uint8_t CurrentProcState    = ledMatProcessInstance.LedMatProcState;
  uint16_t WaitTimeRet = 100; // Set 100ms as default durration
  switch (CurrentProcInstance) {
    case P1_CLEAR_DISPLAY:
      if (CurrentProcState == LEDMATPROC_FINISHED) {
        ledMatProcessInstance.LedMatProcID    = P2_SNAKE_AFFECT;
        ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
        
        DebugPrintState[DBSTATEBUFNUM] = CurrentProcInstance+0x30;
        uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
      } else {
        /* Wait for process finish */
      }
      WaitTimeRet = 100; // 100ms
      break;
    case P2_SNAKE_AFFECT:
      if (CurrentProcState == LEDMATPROC_FINISHED) {
        ledMatProcessInstance.LedMatProcID    = P3_NUMCOUNT;
        ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
        DebugPrintState[DBSTATEBUFNUM] = CurrentProcInstance+0x30;
        uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
      } else {
        /* Wait for process finish */
      }
      WaitTimeRet = 50; // 50ms
      break;
    case P3_NUMCOUNT:
      if (CurrentProcState == LEDMATPROC_FINISHED) {
        ledMatProcessInstance.LedMatProcID    = P4_RANDOMNUMBER;
        ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
        DebugPrintState[DBSTATEBUFNUM] = CurrentProcInstance+0x30;
        uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
        ledMatI_ClearDisplay();
      } else {
        /* Wait for process finish */
      }
      WaitTimeRet = 700; // 700ms
      break;
    case P4_RANDOMNUMBER:
      if (CurrentProcState == LEDMATPROC_FINISHED) {
        ledMatProcessInstance.LedMatProcID    = P5_KEEPSINGLECOL;
        ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
        DebugPrintState[DBSTATEBUFNUM] = CurrentProcInstance+0x30;
        uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
      } else {
        /* Wait for process finish */
      }
      WaitTimeRet = 5; // 50ms
      break;
    case P5_KEEPSINGLECOL:
      if (CurrentProcState == LEDMATPROC_FINISHED) {
        ledMatProcessInstance.LedMatProcID    = P6_IDLERUN;
        ledMatProcessInstance.LedMatProcState = LEDMATPROC_READY;
        DebugPrintState[DBSTATEBUFNUM] = CurrentProcInstance+0x30;
        uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
      } else {
        /* Wait for process finish */
      }
      WaitTimeRet = 700;
      break;
    case P6_IDLERUN:
      WaitTimeRet = 50;
      break;
    default:
      ledMatProcessInstance.LedMatProcID = P1_CLEAR_DISPLAY;
      break;
  }
  
  
  return WaitTimeRet;
}





void ledMat_P1ClearMat(void) {
  ledMatProcessInstance.LedMatProcID = P1_CLEAR_DISPLAY;
  ledMatProcessInstance.LedMatProcState = LEDMATPROC_ONGOING;
  ledMatI_ClearDisplay();
  
  DebugPrintState[DBSTATEBUFNUM] = P1_CLEAR_DISPLAY+0x30;
  uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
  
  ledMatProcessInstance.LedMatProcState = LEDMATPROC_FINISHED;
}

void ledMat_P2SetSnakeAffect(void) {
  static uint8_t singleledid = 0;
  static uint8_t singlerowid = 1;
  ledMatProcessInstance.LedMatProcID    = P2_SNAKE_AFFECT;
  ledMatProcessInstance.LedMatProcState = LEDMATPROC_ONGOING;
  
  if (singleledid++ >= 7) {
    //ledMatI_SetSingleRowDisplay(singlerowid, 0x0);
    ledMatI_SetSingleRowDisplay(singlerowid, 0xFF);
    singleledid = 0;
    singlerowid++;
    if (singlerowid>6) {
      singlerowid = 1;
      ledMatProcessInstance.LedMatProcState = LEDMATPROC_FINISHED;
      ledMatI_ClearDisplay();
    }
  }
  //Print_RowCheck(row);
  if (singlerowid % 2 != 0) {
    ledMatI_SetSingleRowIncreaseLedDisplay(singlerowid, singleledid);
  } else {
    ledMatI_SetSingleRowIncreaseInvLedDisplay(singlerowid, singleledid);
  }
  
  DebugPrintState[DBSTATEBUFNUM] = P2_SNAKE_AFFECT+0x30;
  uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
}

void ledMat_P3PrintNumCount(void) {
  static uint8_t printcount = 5;
  ledMatProcessInstance.LedMatProcID = P3_NUMCOUNT;
  if (printcount > 0) printcount--;
  Print_Alphabet(printcount);
  if (printcount == 0) {
    printcount = 5;
    ledMatProcessInstance.LedMatProcState = LEDMATPROC_FINISHED;
  }
  
  DebugPrintState[DBSTATEBUFNUM] = P3_NUMCOUNT+0x30;
  uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc) {
  
}

void ledMat_P4RandomNumberGen(void) {
  static uint8_t randomgentime = 0;
  uint8_t randomCol; // This one value from 1-6 shifting from 0-5 => 3bit
  uint8_t randomColValIndex; // This one value from 0-7           => 3bit
  uint32_t randomadc;
  uint8_t random4sum[11];
  
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 500);
  HAL_ADC_Stop(&hadc1);
  randomadc = HAL_ADC_GetValue(&hadc1);
  
  // Assume random adc is random 32bit, let make it become 3bit
  random4sum[0] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[1] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[2] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[3] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[4] = randomadc & 0x07; randomadc = randomadc >> 3;
  
  for (int i = 0; i < 5; i++) {
    randomCol = randomCol + random4sum[i];
  }
  randomCol &= 0x07;
  if (randomCol == 0) randomCol = 1;
  if (randomCol == 7) randomCol = 6;
  
  random4sum[5] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[6] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[7] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[7] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[9] = randomadc & 0x07; randomadc = randomadc >> 3;
  random4sum[10]= random4sum[0];
  for (int j = 5; j <= 10; j++) {
    randomColValIndex = randomColValIndex + random4sum[j];
  }
  randomColValIndex &= 0x7;
  
  ledMatI_SetSingleRowSingleLedDisplay(randomCol, randomColValIndex);
  
  if (randomgentime++ >= 100) {
    ledMatProcessInstance.LedMatProcState = LEDMATPROC_FINISHED;
  }
  
  ledMatProcessInstance.LedMatProcID = P4_RANDOMNUMBER;
  
  DebugPrintState[DBSTATEBUFNUM] = P4_RANDOMNUMBER+0x30;
  uartDebugI_PrintDebugInfo(DebugPrintState, DBSTATEBUFSIZE);
}

uint8_t genrandom3bit(void) {
  uint32_t randomadc;
  uint8_t random4sum[3];
  
  uint8_t randomvalret;
  
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 500);
  HAL_ADC_Stop(&hadc1);
  randomadc = HAL_ADC_GetValue(&hadc1);

    // Assume random adc is random 32bit, let make it become 3bit
    random4sum[0] = randomadc & 0x07; randomadc = randomadc >> 3;
    random4sum[1] = randomadc & 0x07; randomadc = randomadc >> 3;
    random4sum[2] = randomadc & 0x07; randomadc = randomadc >> 3;
    for (int i = 0; i < 3; i++) {
      randomvalret = randomvalret + random4sum[i];
    }
    
    while (randomvalret > 6) {
      randomvalret = 6;
    }
    return randomvalret;
}

uint8_t colselectedid[5];
uint8_t check4existing(uint8_t data) {
  uint8_t retstate = 0;
  for (int i = 0; i < 5; i++) {
    if (data == colselectedid[i]) retstate = 1;
  }
  return retstate;
}

uint8_t colremvbuff[12]         = "ColRemove_X\n";
uint8_t counterselectedbuff[12] = "Selected__X\n";
uint8_t partialrandombuff[14]   = "PartialRandom\n";
void ledMat_P5KeepSingleLed(void) {
  // In this state, only one collum will be keep, other will be clear
  // Entering this state, 6 colum with single led are available
  // Now clear 5 of them
  // [1, 2, 3, 4, 5, 6]
  
  uint8_t col2clear[6] = {1,2,3,4,5,6};
  static uint8_t colselectedid_index = 0;
  
  static uint32_t timecountrandom = 0;
  static uint8_t partialrandom = 0;
  
  uint8_t colremove;

  
  
  colremove = genrandom3bit();

  if (timecountrandom < 15) {
    if (1 != check4existing(colremove)) {
      // No repeat found, then just remove
      colselectedid[colselectedid_index] = colremove;
      ledMatI_SetSingleRowDisplay(colremove, 0x00);
    
      colremvbuff[10] = colremove+0x30;
      uartDebugI_PrintDebugInfo(colremvbuff,12);
    
      colselectedid_index++;
    } else {
      // Countup partical random remove
      timecountrandom++;
    }
  } else {
    // do partial random
    uartDebugI_PrintDebugInfo(partialrandombuff,14);
    if (1 != check4existing(partialrandom)) {
      // No repeat found, then just remove
      colselectedid[colselectedid_index] = partialrandom;
      ledMatI_SetSingleRowDisplay(partialrandom, 0x00);
    
      colremvbuff[10] = partialrandom+0x30;
      uartDebugI_PrintDebugInfo(colremvbuff,12);
    
      colselectedid_index++;
      
    } else {
      partialrandom++;
    }
  }
  
  counterselectedbuff[10] = colselectedid_index+0x30;
  uartDebugI_PrintDebugInfo(counterselectedbuff,12);
  if (colselectedid_index == 5) {
    ledMatProcessInstance.LedMatProcState = LEDMATPROC_FINISHED;
  }
}

void ledMatI_UpdateData(void) {
  /* This task will execute the process */
  uint8_t CurrentProcInstance = ledMatProcessInstance.LedMatProcID;

  switch (CurrentProcInstance) {
    case P1_CLEAR_DISPLAY:
      ledMat_P1ClearMat();
      break;
    case P2_SNAKE_AFFECT:
      ledMat_P2SetSnakeAffect();
      break;
    case P3_NUMCOUNT:
      ledMat_P3PrintNumCount();
      break;
    case P4_RANDOMNUMBER:
      ledMat_P4RandomNumberGen();
      break;
    case P5_KEEPSINGLECOL:
      ledMat_P5KeepSingleLed();
      break;
    case P6_IDLERUN:
      break;
    default:
      break;
  }
}



