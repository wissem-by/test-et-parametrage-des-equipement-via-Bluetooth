
/******************** (C) COPYRIGHT 2008 AlphaConnectInternational ********************
* File Name          : 
* Author             : MASMOUDI Mohamed 
* Version            : V1.0
* Date               : 
* Description        : 
****************************************************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "stm32g4xx_ll_gpio.h" 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private Variable ----------------------------------------------------------*/
/* Private Function ----------------------------------------------------------*/
void LED_init(ledStruct * led,uint8_t inverted, uint8_t defaultStatus,GPIO_TypeDef* port,uint16_t pin,uint8_t period,uint8_t duty){
// GPIO_InitTypeDef GPIO_InitStructure;
    led->pin=pin;
    led->port=port;
    /* Configure led as output push-pull */
    /*GPIO_InitStructure.GPIO_Pin = led->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(led->port, &GPIO_InitStructure);*/
    led->inverted=inverted;
        led->period=period;
        led->duty=duty;
        LED_setBlocked(led,0);
    LED_setStatus(led,defaultStatus);
   
}
 
void LED_setStatus(ledStruct * led,uint8_t status){

        led->status=status;
        //GPIO_WriteBit(led->port,led->pin,(BitAction)  status);
        if(status){
          LL_GPIO_SetOutputPin(led->port, led->pin);
        }else{
          LL_GPIO_ResetOutputPin(led->port, led->pin);
        }
}
uint8_t LED_getStatus(ledStruct * led){
    return led->status;
}
void LED_invert(ledStruct * led){
    if(!led->blocked)
        LED_setStatus(led,!LED_getStatus(led));
}
void LED_ForceInvert(ledStruct * led){
    LED_setStatus(led,!LED_getStatus(led));
}
void LED_setBlocked(ledStruct * led,uint8_t blocked){
    led->blocked=blocked;
}
uint8_t LED_getBlocked(ledStruct * led){
    return led->blocked;
}
void LED_setDuty(ledStruct * led,uint16_t duty){
        led->duty=(uint8_t)led->period*duty/100;
}
void LED_setPeriod(ledStruct * led,uint8_t period){
        if(led->period!=period){
            led->duty=(uint8_t)(led->duty*(uint16_t)100/led->period);//save percent duty
            led->period=period;
            led->duty=(uint8_t)((uint16_t)led->period*led->duty/100);//restore duty
        }
}
void LED_Routine(ledStruct * led){
    if(LED_getBlocked(led)==0){
        led->counter++;
        if(led->counter>=led->period){
            LED_setStatus(led,1);
            led->counter=0;
        }
        if(led->counter==led->duty)
            LED_setStatus(led,0);
    }
}
void LED_startAfterDuty(ledStruct * led){
  led->counter=led->duty;
}
 
/******************* (C) COPYRIGHT 2008 ACI ********END OF FILE****/