#include "machine.h"
#include "stm32g4xx_ll_gpio.h"


void machineLowLevelInit(){
  gpio_conf();

}

void machineInit(machine_struct * machine){
  machine->globalTimeOut=900000;//15min
  machineLowLevelInit();
  sim908_init(&machine->sim908);
  data_flash_init(&machine->data_flash);
}


__IO uint16_t msCounter=0;
void machineDecDelay(machine_struct * machine){
  if(machine->globalTimeOut)
    machine->globalTimeOut--;
   if(machine->time!=0)
    machine->time--;
   sim908DecDelay(&machine->sim908);
   msCounter++;
   if(msCounter>=100){
     machine->tick100ms=1;
     msCounter=0;
   }
    /*if(msCounter>=10){
     machine->tick10ms=1;
     msCounter=0;
   }*/
}

void machineDelay(machine_struct * machine,uint32_t test){
  machine->time=test;
  while(machine->time!=0);
}

