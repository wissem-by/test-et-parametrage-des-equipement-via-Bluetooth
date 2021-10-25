
/******************** (C) COPYRIGHT 2008 AlphaConnectInternational ********************
* File Name          : 
* Author             : MASMOUDI Mohamed 
* Version            : V1.0
* Date               : 
* Description        : 
****************************************************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct{
    uint8_t blocked;
    uint8_t status;
    uint16_t pin;
    GPIO_TypeDef* port;
    uint8_t inverted;
        uint8_t period;
        uint8_t duty;
        uint8_t counter;
}ledStruct;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private Variable ----------------------------------------------------------*/
/* Private Function ----------------------------------------------------------*/
void LED_init(ledStruct * led,uint8_t inverted, uint8_t defaultStatus,GPIO_TypeDef* port,uint16_t pin,uint8_t period,uint8_t duty);
void LED_setStatus(ledStruct * led,uint8_t status);
void LED_invert(ledStruct * led);
void LED_ForceInvert(ledStruct * led);
uint8_t LED_getStatus(ledStruct * led);
void LED_setBlocked(ledStruct * led,uint8_t blocked);
uint8_t LED_getBlocked(ledStruct * led);
void LED_Routine(ledStruct * led);
void LED_setDuty(ledStruct * led,uint16_t duty);
void LED_setPeriod(ledStruct * led,uint8_t period);
/******************* (C) COPYRIGHT 2008 ACI ********END OF FILE****/