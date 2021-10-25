/******************** (C) COPYRIGHT 2008 AlphaConnectInternational ********************
* File Name          : sim508_GPS.c
* Author             : OUESLATI Mohamed Salah
* Version            : V2.0
* Date               : 10/2009
* Description        : Library de l'utiliosation du flash pour le AJAJA 3.1
****************************************************************************/

/* Defines ------------------------------------------------------------------*/
#define C_FLASH 1

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx.h"
#include <string.h>
#include "machine.h"
#include "stm32g4xx_hal_flash.h"
#include "flash.h"
/* Private variables ---------------------------------------------------------*/  

/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : .
* Description    : .
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t  AJAJA_FLASH_Init(){
   //Unlock the Flash Program Erase controller
   //HAL_FLASH_Unlock();
   HAL_FLASH_Unlock();
   __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
   // Clear All pending flags 
   //FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
   //FLASH_ReadOutProtection(ENABLE);
   //FLASH_UserOptionByteConfig(OB_IWDG_SW,OB_STOP_NoRST,OB_STDBY_NoRST);
    return 1;
}

void FLASH_G4_ErasePage(uint8_t pageNum,uint8_t bank){
  FLASH_EraseInitTypeDef fe;
  fe.NbPages=1;
  fe.TypeErase=FLASH_TYPEERASE_PAGES;
  fe.Banks=bank;
  fe.Page=pageNum;
  uint32_t error;
  HAL_FLASHEx_Erase(&fe,&error);
}
/*******************************************************************************
* Function Name  : .
* Description    : .
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
/*void AJAJA_WriteXWord(uint32_t AddressDeb,uint8_t Length ,uint64_t * XWord)
{
   HAL_StatusTypeDef FLASHStatus;
   uint8_t NbrWord;
   for(NbrWord=0;NbrWord<Length;NbrWord+=2)
   {
      FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,AddressDeb,*(XWord+NbrWord));
      AddressDeb+=8;
   }
}*/
/*******************************************************************************
* Function Name  : .
* Description    : .
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef SFM
void AJAJA_SF_WriteXWord(uint32_t AddressDeb,uint16_t Length ,uint8_t * XWord)
{
    sFLASH_SaveData(AddressDeb,XWord, Length);
}
#endif
/******************* (C) COPYRIGHT 2008 ACI ********END OF FILE****/
