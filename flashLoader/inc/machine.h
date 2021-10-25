//#include "hw_config.h"
//#include "usb_lib.h"
//#include "usb_desc.h"
//#include "usb_pwr.h"
#include "sim908.h"
//#include "lineCoder.h"




typedef struct {
sim908_struct sim908;
data_flash_struct data_flash;
__IO uint32_t time;
__IO uint8_t tick100ms;
__IO uint32_t globalTimeOut;
}machine_struct;

void machineInit(machine_struct * machine);
void machineLowLevelInit();
void openSim908();
void machineDelay(machine_struct * machine,uint32_t test);
void machineDecDelay(machine_struct * machine);