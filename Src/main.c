/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#define BUILDTIME __TIME__
#define BUILDDATE __DATE__
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "machine.h"
#include "led.h"
#include "stm32g4xx_hal_flash.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t check;
__IO  char test[64];
machine_struct machine;
ledStruct  led;
extern RTC_TypeDef RTC1;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_IWDG_Init(void);
//void AJAJA_WriteXWord(uint32_t AddressDeb,uint8_t Length ,uint64_t * XWord);
void AJAJA_WriteXWord(uint32_t AddressDeb,uint8_t Length ,uint32_t * XWord);
//void sup(void);
/* USER CODE BEGIN PFP */

 
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//#define DEBUG
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
//uint32_t URL_LOCATION =0x801F000 ;
char SIM908_SETUP_TAB[SIM908_SETUP_NBR][40];
char SIM908_FTPCONF_TAB[SIM908_FTPCONF_NBR][64];
//#define DEBUG
int main(void)
{
  /* USER CODE BEGIN 1 */
  SCB->VTOR = (uint32_t)0x8000000;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral 
  */
  //LL_PWR_DisableDeadBatteryPD();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SysTick_Config(SystemCoreClock/1000);
  openSim908();
  //sim908_loadFtpConfig(&machine.sim908); 
  machineInit(&machine);
  LED_init(&led, 0,  0,GPIOB,LL_GPIO_PIN_7,10,0);  
  LED_setDuty(&led,20);
  machineDelay(&machine,2000);
  
//LL_RTC_BKP_SetRegister(&RTC1,LL_RTC_BKP_DR0,0xbb);
#ifndef DEBUG
   /* if(LL_RTC_BKP_GetRegister(&RTC1,LL_RTC_BKP_DR0)!=0xbb)     // BKP_ReadBackupRegister(BKP_DR9)
  {
    jump(ARD_FLASH_PROG_START);
  }*/
#endif
    //******************* WWDG init **************
#ifndef DEBUG
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);
  LL_IWDG_SetReloadCounter(IWDG,0xFFF);
  LL_IWDG_ReloadCounter(IWDG);
#endif
  //********************************************
  LL_RTC_BKP_SetRegister(&RTC1,LL_RTC_BKP_DR0, 0xAC); //BKP_WriteBackupRegister(BKP_DR9,0xAC)
  //HAL_FLASH_Unlock();//ouvrir flash
 // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_PROGERR|FLASH_FLAG_WRPERR); //FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR
  AJAJA_FLASH_Init(); 
  supp_flash(ARD_FLASH_DATA_START,NBR_PAGE_APP);
  //FLASH_G4_ErasePage(62,1);
 // AJAJA_WriteXWord(0x8002000,1,(uint32_t *)0x8010000);
 // uint64_t marker=0xBBBBBBBBBBBBBBBB;
  //AJAJA_WriteXWord(0x8010000,1,(uint32_t *)&marker);
 
        //HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,flash_i+ARD_FLASH_IAP_START,data_flash->testdata);//FLASH_ProgramWord(flash_i+ARD_FLASH_IAP_START,data_flash->testdata);
      //}
        //uint32_t time=5000;
        //while(time!=0);
  //machine.data_flash.stored=1;
  while (1)
  {   if(!machine.globalTimeOut){
      exitWithError(ARD_FLASH_PROG_START,GLOBALE_TIME);
    }
    /*check=LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_0);   
    if(!check){
     openSim908(&machine);
     machineInit(&machine);
    }*/
    if(machine.tick100ms){
      LED_Routine(&led);
      machine.tick100ms=0;
    }
     //GPIO_SetBits(GPIOA, GPIO_Pin_8);
     cmd_routine(&machine.sim908.cmd);
     test_routine(&machine.sim908,&machine.data_flash);
    flash_store(&machine.data_flash);
     
     //**********************wwdog
#ifndef DEBUG
     LL_IWDG_ReloadCounter(IWDG); //  IWDG_ReloadCounter();
#endif
     if(machine.data_flash.stored==1){
       __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
       //sup();
       supp_flash(ARD_FLASH_PROG_START, NBR_PAGE_APP);
       uint32_t flash_i;
       for(flash_i=(NBR_PAGE_IAP*PAGE_SIZE);flash_i < 0x10000 ;flash_i=flash_i+8){//4
          //machine.data_flash.testdata=*(uint32_t*)(flash_i+(ARD_FLASH_DATA_START-ARD_FLASH_IAP_START-(NBR_PAGE_IAP*PAGE_SIZE))+ARD_FLASH_IAP_START);//****  uint32_t        
          AJAJA_WriteXWord(flash_i+ARD_FLASH_IAP_START,1,(uint32_t *)(flash_i+(ARD_FLASH_DATA_START-ARD_FLASH_IAP_START-(NBR_PAGE_IAP*PAGE_SIZE))+ARD_FLASH_IAP_START));//machine.data_flash.testdata
       }
        // AJAJA_FLASH_Init(); 
       //__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);  
       //sup();
     //uint32_t flash_i;
     /*for(flash_i=(NBR_PAGE_IAP*PAGE_SIZE);flash_i < data_flash->adrCurr ;flash_i=flash_i+8){//4
        data_flash->testdata=*(uint32_t*)(flash_i+(ARD_FLASH_DATA_START-ARD_FLASH_IAP_START-(NBR_PAGE_IAP*PAGE_SIZE))+ARD_FLASH_IAP_START);//****  uint32_t        
        AJAJA_WriteXWord(flash_i+ARD_FLASH_IAP_START,1,(uint32_t *)data_flash->testdata);
        //HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,flash_i+ARD_FLASH_IAP_START,data_flash->testdata);//FLASH_ProgramWord(flash_i+ARD_FLASH_IAP_START,data_flash->testdata);
      }*/
     exitWithSuccess(ARD_FLASH_PROG_START);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
}
/* void sup(void)
 {   for(uint8_t i=4;i<28+4;i++){
  
         FLASH_G4_ErasePage(i,1);//FLASH_ErasePage(adr_debut + i* 0x400); 
       }
 }*/
/*void AJAJA_WriteXWord(uint32_t AddressDeb,uint8_t Length ,uint64_t * XWord)
{
   uint8_t NbrWord;
   for(NbrWord=0;NbrWord<Length;NbrWord+=2)
   {
    // FLASH_ProgramWord(AddressDeb,*(XWord+NbrWord));
     HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,AddressDeb,*(XWord+NbrWord*4));
      AddressDeb+=8;
     
   }
}*/
uint64_t tmp=0;;
void AJAJA_WriteXWord(uint32_t AddressDeb,uint8_t Length ,uint32_t * XWord)
{
  //HAL_StatusTypeDef FLASHStatus;
   for(uint8_t i =0;i<Length;i++)
   { 
      tmp=((uint64_t)(XWord[i*2+1])<<32)|XWord[i*2]; 
      //FLASHStatus = 
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,AddressDeb,tmp);
      AddressDeb+=8;
   }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_8);
  LL_PWR_EnableRange1BoostMode();
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {
    
  }
  LL_PWR_EnableBkUpAccess();
  //LL_RCC_ForceBackupDomainReset();
  //LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 85, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  /* Insure 1µs transition state at intermediate medium speed clock based on DWT */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;
  while(DWT->CYCCNT < 100);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(170000000);

  LL_SetSystemCoreClock(170000000);
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);//PRESCALER_4
  LL_IWDG_SetWindow(IWDG, 4095);
  LL_IWDG_SetReloadCounter(IWDG, 4095);
  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_ReloadCounter(IWDG);    
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  LL_RTC_InitTypeDef RTC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC and set the Time and Date 
  */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
  
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  /**USART3 GPIO Configuration  
  PB10   ------> USART3_TX
  PB11   ------> USART3_RX 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP; //PULL_NO
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(USART3);
  LL_USART_ConfigAsyncMode(USART3);

  /* USER CODE BEGIN WKUPType USART3 */

  /* USER CODE END WKUPType USART3 */

  LL_USART_Enable(USART3);

  /* Polling USART3 initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART3))) || (!(LL_USART_IsActiveFlag_REACK(USART3))))
  {
  }
  /* USER CODE BEGIN USART3_Init 2 */
  LL_USART_EnableIT_RXNE(USART3);
  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void openSim908(){

      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1); // GPIO_ResetBits(GPIOB, GPIO_Pin_1);
      machineDelay(&machine,900);
      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1); //GPIO_SetBits(GPIOB, GPIO_Pin_1);
       //machineDelay(&machine,3000);
      
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
