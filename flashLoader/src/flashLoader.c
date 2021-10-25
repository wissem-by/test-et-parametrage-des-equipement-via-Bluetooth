#include "flashLoader.h"
#include "stm32g4xx_ll_rtc.h"
#include "stm32g4xx_hal_flash.h"
pfunction adr ;
uint32_t JumpAddress;
//#include "machine.h"
//machine_struct machine;
//char buff_2[4];

void flash_store(data_flash_struct * data_flash){
  lineCoder line;
  uint32_t flash_i;
  switch(data_flash->state){
    case FLASH_IDLE:
      if(data_flash->ready==1){
        data_flash->state=FLASH_LINE_SAVE;
        data_flash->ready=0;
      }
      //load and jump
      /*if(data_flash->stored==1){
        data_flash->state=FLASH_STORE;
        data_flash->stored=0;
      }*/
      break;
    case FLASH_LINE_SAVE:
      if(LC_build(&line,data_flash->dataFlash)&&line.type==0){
        if((uint32_t )line.adresse >= 0x10000){//tessssst 0x10 000
          exitWithError(ARD_FLASH_PROG_START,ADRESS_ERROR);
        }
        
        if (line.adresse>=RELATIVE_PROG_STAR){
          //char buff_2[4];
          //strncpy(buff_2,data_flash->dataFlash[3],4);
          /*if(buff_2=="0000"){
            data_flash->stored=1;
          }*/
            
          if(line.adresse==data_flash->adrCurr){
              LC_save(&line,(ARD_FLASH_DATA_START-ARD_FLASH_IAP_START-(NBR_PAGE_IAP*PAGE_SIZE)));// a modifier
             data_flash->adrCurr+=line.length;
           }else{
             //exit with error
             exitWithError(ARD_FLASH_PROG_START,ADRESS_ERROR);
            }
        }
      }/*else if(line.type==0){
            //exit with error
           exitWithError(ARD_FLASH_PROG_START);        
      }*/
      
       data_flash->state=FLASH_IDLE;
       data_flash->ready=0;
      break;
    //case FLASH_STORE:
     //supp flashk
      //AJAJA_FLASH_Init();
      //supp_flash(ARD_FLASH_PROG_START, NBR_PAGE_APP);

      //supp_flash
      //machineDelay(&machine,2000);
     /* for(flash_i=(NBR_PAGE_IAP*PAGE_SIZE);flash_i < data_flash->adrCurr ;flash_i=flash_i+8){//4
        data_flash->testdata=*(uint32_t*)(flash_i+(ARD_FLASH_DATA_START-ARD_FLASH_IAP_START-(NBR_PAGE_IAP*PAGE_SIZE))+ARD_FLASH_IAP_START);//****  uint32_t        
        AJAJA_WriteXWord(flash_i+ARD_FLASH_IAP_START,1,(uint32_t *)data_flash->testdata);
        //HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,flash_i+ARD_FLASH_IAP_START,data_flash->testdata);//FLASH_ProgramWord(flash_i+ARD_FLASH_IAP_START,data_flash->testdata);
      }
       data_flash->stored=0;
       exitWithSuccess(ARD_FLASH_PROG_START);
      break;*/
    }
}

void data_flash_init(data_flash_struct * data_flash){
  data_flash->ready=0;
  data_flash->stored=0;
  data_flash->state=FLASH_IDLE;
  data_flash->adrCurr=(NBR_PAGE_IAP*PAGE_SIZE);
}

void jump(uint32_t test){
   //FLASH_Unlock();
  // if(((*(volatile uint32_t*) test) & 0x2FFE0000) == 0x20000000) {
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000) ;
          JumpAddress = *(volatile uint32_t*) (test + 4);
          adr = (pfunction)JumpAddress;
          __set_MSP(*(volatile uint32_t*) test);
          adr();
        //}
}

RTC_TypeDef RTC1;

void exitWithError(uint32_t test,uint8_t code){
  //write ac
  LL_RTC_BKP_SetRegister(&RTC1,LL_RTC_BKP_DR0, 0xAA-code); //BKP_WriteBackupRegister(BKP_DR9,0xAA-code);
  supp_flash(ARD_FLASH_DATA_START, NBR_PAGE_APP);//**/
  NVIC_SystemReset();
  // jump(test);
}


void exitWithSuccess(uint32_t test){
  //write ab
   LL_RTC_BKP_SetRegister(&RTC1,LL_RTC_BKP_DR0, 0xAB);//BKP_WriteBackupRegister(BKP_DR9,0xAB);
   supp_flash(ARD_FLASH_DATA_START, NBR_PAGE_APP);
   NVIC_SystemReset();
   //jump(test);
}