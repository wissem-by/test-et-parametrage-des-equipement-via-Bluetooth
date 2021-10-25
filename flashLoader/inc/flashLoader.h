#include "stm32g4xx.h"
#include "lineCoder.h"


#define URL_LOCATION 0x801e808  //0x801f804
#define ARD_FLASH_IAP_START  0x8000000
#define ARD_FLASH_DATA_START  0x8010000
#define NBR_PAGE_PROG 32  //64
#define NBR_PAGE_IAP  4   //8
#define NBR_PAGE_APP  28  //56
#define PAGE_SIZE 0x800   //0x400
#define ARD_FLASH_PROG_START ARD_FLASH_IAP_START+(NBR_PAGE_IAP*PAGE_SIZE)
#define MAX_PROG_SIZE PAGE_SIZE*(NBR_PAGE_PROG-NBR_PAGE_IAP)
#define RELATIVE_PROG_STAR NBR_PAGE_IAP*PAGE_SIZE

#define CONNECT_ERROR   0
#define ADRESS_ERROR    1
#define FLASH_ERROR     2
#define ERASE_ERROR     3
#define PROFILE_ERROR   4
#define SEND_ERROR      5
#define GLOBALE_TIME    6

typedef  void (*pfunction)(void);

typedef enum{FLASH_IDLE,FLASH_LINE_SAVE,FLASH_STORE}data_flash_state;
typedef struct{
  data_flash_state state;
  char dataFlash[64];
 // lineCoder line;
  uint32_t adrCurr;
  uint8_t ready;
  uint8_t stored;
  uint64_t testdata;//uint32_t
}data_flash_struct;

void flash_store(data_flash_struct * data_flash);
void data_flash_init(data_flash_struct * data_flash);
void jump(uint32_t);
void exitWithSuccess(uint32_t);
void exitWithError(uint32_t test,uint8_t code);