#include "sim908.h"
#include "led.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_usart.h"
#include "misc.h"
#include "utiles.h"
//********************************remplissage des cmds de sim908_setup*********/////
extern char SIM908_SETUP_TAB[SIM908_SETUP_NBR][40];
// char SIM908_SETUP_RESP[SIM908_SETUP_NBR][20]={ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK};
// uint32_t SIM908_SETUP_TIMEOUT[SIM908_SETUP_NBR]={1000,1000,2000,2000,2000,2000,2000,2000,2000,10000};




//********************************remplissage des cmds de ftp-conf*********/////
extern char SIM908_FTPCONF_TAB[SIM908_FTPCONF_NBR][64];
// char SIM908_FTPCONF_RESP[SIM908_FTPCONF_NBR][20]={ch_OK,ch_OK,ch_OK,ch_OK,ch_OK,ch_OK};
// uint32_t SIM908_FTPCONF_TIMEOUT[SIM908_FTPCONF_NBR]={2000,2000,2000,2000,2000,2000};
 char  connected_ID[10];
 char DATA[64];
   int k=0;
   int m=0;
   int msg_lenght;

extern ledStruct  led;
/*void bkp_init(){
  //**********bkp
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
 }*/
void gpio_conf(){
  /*GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB , ENABLE);
 
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3  ;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); */
  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1); //GPIO_SetBits(GPIOB,GPIO_Pin_1); 
  
  //***************usart3
 /* GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//input from sim908
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); */
}


 /* void SIM908_Usart_Init(){
   USART_InitTypeDef  USART_InitStructure ;
   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE );
  USART_InitStructure.USART_BaudRate =57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
 
  
  USART_Init(USART3, &USART_InitStructure);
 
   //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);   
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
   
   USART_Cmd(USART3, ENABLE);
} */

void SIM908_Nvic_Config(){

}

void usart_send(char * data)
{
   uint8_t i=0;
   strcat(data,"\r\n");
   while(i<strlen(data)){ 
    while (!LL_USART_IsActiveFlag_TC(USART3))
    {
        //NOP_Process();
    }
    LL_USART_TransmitData8(USART3,data[i]);   //USART_SendData(USART3,data[i])
    // while(!LL_USART_IsActiveFlag_TXE_TXFNF(USART3));//****    USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET
     i++;
   }
}
/*void usart_send(char * data)
{
   uint8_t i=0;
   while(i<strlen(data)){       
    LL_USART_TransmitData8(USART3,data[i]);   //USART_SendData(USART3,data[i])
     while(!LL_USART_IsActiveFlag_TXE_TXFNF(USART3));//****    USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET
     i++;
   }
}*/
const char ch_OK[]="OK";
const char CH_AT_READ[] = "AT+QFREAD=134072";

char command[64];
 char * connectedID[10];
//
void sim908_receiveChar(sim908_struct * sim908,uint8_t c){ 
  if((c=='\n') && sim908->cmd.cursor_buff>0){ 
    sim908->cmd.buffer[sim908->cmd.cursor_buff]='\0';
    sim908->cmd.done=1;
    if(strstr((char*)sim908->cmd.buffer,ch_OK)||strstr((char*)sim908->cmd.buffer,"ERROR")){
       strcpy((char*)sim908->cmd.repState,(char*)sim908->cmd.buffer);
       sim908->cmd.doneState=1;
    }else if(strstr((char*)sim908->cmd.buffer,"+")||strstr((char*)sim908->cmd.buffer,"CONNECT")){
       strcpy((char*)sim908->cmd.repInfo,(char*)sim908->cmd.buffer);
       sim908->cmd.doneInfo=1;
    }else{
       strcpy((char*)sim908->cmd.repData,(char*)sim908->cmd.buffer);
       sim908->cmd.doneData=1;
    }
    sim908->cmd.cursor_buff=0;
  }else{
    if(c!='\r'&&c!='\0'&&c!='\n')
    sim908->cmd.buffer[sim908->cmd.cursor_buff++]=c;
  }

}

void cmd_routine(cmd_struct * cmd){
  switch(cmd->state){
    case CMD_IDLE:
      cmd->doneData=0;
      cmd->doneInfo=0;
      cmd->doneState=0;
      cmd->done=0;
      if(cmd->ready==1){
        usart_send(cmd->str);
        cmd->cursor_buff=0;
        cmd->got =0;
        cmd->time=cmd->time_out;
        cmd->state=CMD_WAIT;
      }
      break;
    case CMD_WAIT:
      if(cmd->got ==1){
        cmd->state=CMD_IDLE;
        cmd->ready=0;
      }
      if(!cmd->time){
        cmd->done=2;
        cmd->ready=0;
        cmd->state=CMD_IDLE;
      }
      break;
  }
}

void cmd_init(cmd_struct*cmd){
  cmd->state =CMD_IDLE;
  cmd->ready=0;
  cmd->time_out=2000;
 }

void test_routine(sim908_struct* sim908,data_flash_struct *data_flash){
  char lineLenth[3];
  char getString[20];
  if(data_flash->state==FLASH_LINE_SAVE)
    return;
  switch(sim908->state){
    case SIM908_INIT:
      if(!sim908->timeout){
        sim908->nbrTrySimInit++;
        if(sim908->nbrTrySimInit>1){
         //exit with error
          exitWithError(ARD_FLASH_PROG_START,CONNECT_ERROR);
        }else{
          sim908_send(sim908,"AT",1000);  //\r\n
          sim908->state=SIM908_INIT_RESP;
        }
        sim908->timeout=2000;
      }
      break;
    case SIM908_INIT_RESP:
      if(sim908->cmd.doneState==1){
        
        if(strstr((char*)sim908->cmd.repState,ch_OK)){
          sim908->cmd.got=1;
          sim908->state=QUECTEL_BT_STATE;
          //sim908->state=SIM908_WR_NET;
          
          LED_setDuty(&led,50);
        }
      }
      if(sim908->cmd.done==2){
        sim908->state=SIM908_INIT;
        sim908->cmd.ready=0;
      }
      break;
    case QUECTEL_BT_STATE:
      sim908_send(sim908,"AT+QBTPWR?",1000);  //\r\n
      sim908->state=QUECTEL_BT_STATE_RESP;     
      break;
    case QUECTEL_BT_STATE_RESP:
         if(sim908->cmd.doneState==1){
          if(strstr((char*)sim908->cmd.repState,ch_OK)){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->cmd.nbrTry=0;
              if(strstr((char*)sim908->cmd.repInfo,"+QBTPWR: 1")){          
                sim908->state=QUECTEL_BT_LISTEN;
              }else{
                sim908->state=QUECTEL_BT_PAIR_CNF;     
              }
          }else{
            sim908->state=QUECTEL_BT_STATE;
            sim908->cmd.ready=0;
            
          }
         }
         if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_STATE;
            sim908->cmd.ready=0;
         }
         break;
  
    case QUECTEL_BT_PAIR_CNF:
      sim908_send(sim908,"AT+QBTCFG=\"pair\",0",1000);  //\r\n
      sim908->state=QUECTEL_BT_PAIR_CNF_RESP;
     
      break;
    case QUECTEL_BT_PAIR_CNF_RESP:
          if(strstr((char*)sim908->cmd.repState,ch_OK)){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_POWER;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_PAIR_CNF;
            sim908->cmd.ready=0;
          }
         
          if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_PAIR_CNF;
            sim908->cmd.ready=0;
         }
         break;
     
    case QUECTEL_BT_POWER:
      sim908_send(sim908,"AT+QBTPWR=1",300);  //\r\n
      sim908->state=QUECTEL_BT_POWER_RESP;
     
      break;
    case QUECTEL_BT_POWER_RESP:
         if(sim908->cmd.doneState==1){
          if(strstr((char*)sim908->cmd.repState,ch_OK)){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_LISTEN;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_POWER;
            sim908->cmd.ready=0;
          }
         }
          if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_POWER;
            sim908->cmd.ready=0;
         }
         break;
     
    case QUECTEL_BT_LISTEN:
          if(strstr((char*)sim908->cmd.repInfo,"+QBTIND: \"pair\"")){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_ACPT_PAIR;
          }else if(strstr((char*)sim908->cmd.repInfo,"+QBTIND: \"conn\"")) {
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_ACPT_CONN;
          }else if(strstr((char*)sim908->cmd.repInfo,"+QBTIND: \"recv\"")) {
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_READ;
          }
         break;
         
     case QUECTEL_BT_READ:
        sim908_send(sim908,"AT+QSPPREAD=2,1500",1000);  //\r\n
        
        sim908->state=QUECTEL_BT_READ_RESP;
        
      break;
     case QUECTEL_BT_READ_RESP:
       
        if(sim908->cmd.doneState==1){
          if(strstr((char*)sim908->cmd.repState,ch_OK)){
            if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+IP?")) && (strcmp(sim908->ftp.url,""))){
                strcpy(DATA,sim908->ftp.url);
                msg_lenght = strlen(sim908->ftp.url);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+IP="))){
                      strcpy(sim908->ftp.url,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.url[0], &sim908->ftp.url[6], strlen(sim908->ftp.url) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }
            else if (!(strcmp((char*)sim908->cmd.repData,"AJ"))){
              msg_lenght = 2;
                strcpy(DATA,"OK");
                sim908->state=QUECTEL_BT_SEND;
            }
            
            else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+PORT?")) && (strcmp(sim908->ftp.port,""))){
                strcpy(DATA,sim908->ftp.port);
                msg_lenght = strlen(sim908->ftp.port);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+PORT="))){
                      strcpy(sim908->ftp.port,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.port[0], &sim908->ftp.port[8], strlen(sim908->ftp.port) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+APN?")) && (strcmp(sim908->ftp.apn,""))){
                strcpy(DATA,sim908->ftp.apn);
                msg_lenght = strlen(sim908->ftp.apn);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+APN="))){
                      strcpy(sim908->ftp.apn,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.apn[0], &sim908->ftp.apn[7], strlen(sim908->ftp.apn) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+APNUSER?")) && (strcmp(sim908->ftp.apnUser,""))){
                strcpy(DATA,sim908->ftp.apnUser);
                msg_lenght = strlen(sim908->ftp.apnUser);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+APNUSER="))){
                      strcpy(sim908->ftp.apnUser,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.apnUser[0], &sim908->ftp.apnUser[11], strlen(sim908->ftp.apnUser) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+APNPASSWD?")) && (strcmp(sim908->ftp.apnPasswd,""))){
                strcpy(DATA,sim908->ftp.apnPasswd);
                msg_lenght = strlen(sim908->ftp.apnPasswd);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+APNPASSWD="))){
                      strcpy(sim908->ftp.apnPasswd,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.apnPasswd[0], &sim908->ftp.apnPasswd[13], strlen(sim908->ftp.apnPasswd) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+USERNAME?")) && (strcmp(sim908->ftp.username,""))){
                strcpy(DATA,sim908->ftp.username);
                msg_lenght = strlen(sim908->ftp.username);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+USERNAME="))){
                      strcpy(sim908->ftp.username,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.username[0], &sim908->ftp.username[12], strlen(sim908->ftp.username) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+PASSWD?")) && (strcmp(sim908->ftp.passwd,""))){
                strcpy(DATA,sim908->ftp.passwd);
                msg_lenght = strlen(sim908->ftp.passwd);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+PASSWD="))){
                      strcpy(sim908->ftp.passwd,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.passwd[0], &sim908->ftp.passwd[10], strlen(sim908->ftp.passwd) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+FILENAME?")) && (strcmp(sim908->ftp.filename,""))){
                strcpy(DATA,sim908->ftp.filename);
                msg_lenght = strlen(sim908->ftp.filename);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+FILENAME="))){
                      strcpy(sim908->ftp.filename,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.filename[0], &sim908->ftp.filename[12], strlen(sim908->ftp.filename) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+FILESIZE?")) && (strcmp(sim908->ftp.filesize,""))){
                strcpy(DATA,sim908->ftp.filesize);
                msg_lenght = strlen(sim908->ftp.filesize);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+FILESIZE="))){
                      strcpy(sim908->ftp.filesize,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.filesize[0], &sim908->ftp.filesize[12], strlen(sim908->ftp.filesize) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else if (!(strcmp((char*)sim908->cmd.repInfo,"AJ+PATH?")) && (strcmp(sim908->ftp.path,""))){
                strcpy(DATA,sim908->ftp.path);
                msg_lenght = strlen(sim908->ftp.path);
                sim908->state=QUECTEL_BT_SEND;
            }
            else if ((strstr((char*)sim908->cmd.repInfo,"AJ+PATH="))){
                      strcpy(sim908->ftp.path,(char*)sim908->cmd.repInfo);
             
                      memmove(&sim908->ftp.path[0], &sim908->ftp.path[8], strlen(sim908->ftp.path) - 0);
            
                      strcpy((char*)sim908->cmd.repData,"");
                      //sim908_send(sim908,sim908->ftp.url,300);  //\r\n
                      sim908->state=QUECTEL_BT_LISTEN;
            }else{
                strcpy(DATA,"ERROR");
                msg_lenght = 5;
                sim908->state=QUECTEL_BT_SEND;
            }
              
            
            
            
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
           // sim908->state=QUECTEL_BT_SEND;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_READ;
            sim908->cmd.ready=0;//
          }
         } 
          if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_READ;
            sim908->cmd.ready=0;
         }
         break;
         
      case QUECTEL_BT_SEND:
        
        strcat(command,"AT+QSPPSEND=");
        strcat(command,connected_ID);
        //int msg_lenght = strlen((char*)DATA);
        char str[12];
        sprintf(str, "%d", msg_lenght);
        strcat(command,",");
        strcat(command,str);
        sim908_send(sim908,command,1000);  //\r\n
        strcpy(command,"");
        sim908->state=QUECTEL_BT_SEND_RESP;
        
      break;
     case QUECTEL_BT_SEND_RESP:
      //strcpy((char*)DATA,(char*)sim908->cmd.repData);
       while(!(strstr((char*)sim908->cmd.repData,">"))){k++;}
         if(strstr((char*)sim908->cmd.repData,">")){
           strcpy((char*)sim908->cmd.repData,"");
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_MESSAGE;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_SEND;
            sim908->cmd.ready=0;
            
          }
          if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_SEND;
            sim908->cmd.ready=0;
         }
         break;   
     
     case QUECTEL_BT_MESSAGE:
     
      sim908_send(sim908,DATA,300);  //\r\n
      
      sim908->state=QUECTEL_BT_MESSAGE_RESP;
    
      
      break;
     case QUECTEL_BT_MESSAGE_RESP:
         if(sim908->cmd.doneState==1){
          if(strstr((char*)sim908->cmd.repState,ch_OK)){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_LISTEN;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_LISTEN;
            sim908->cmd.ready=0;
            sim908->cmd.nbrTry = 0;
          }
         }
         
         if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_MESSAGE;
            sim908->cmd.ready=0;
         }
         break;
         
         
    case QUECTEL_BT_ACPT_PAIR:
      sim908_send(sim908,"AT+QBTPAIRCNF=1,\"5555\"",500);  //\r\n
      sim908->state=QUECTEL_BT_ACPT_PAIR_RESP;

      break;
    case QUECTEL_BT_ACPT_PAIR_RESP:
         if(sim908->cmd.doneState==1){
          if(strstr((char*)sim908->cmd.repInfo,"+QBTPAIRCNF: 1")){
            sim908->cmd.got=1;
            sim908->cmd.ready=0;
            sim908->state=QUECTEL_BT_LISTEN;
            sim908->cmd.nbrTry = 0;
          }else{
            sim908->state=QUECTEL_BT_ACPT_PAIR;
            sim908->cmd.ready=0;
          }
         }
         
         if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_ACPT_PAIR;
            sim908->cmd.ready=0;
         }
         break;
     
    case QUECTEL_BT_ACPT_CONN:
      sim908_send(sim908,"AT+QBTACPT=1",1000);  //\r\n
      sim908->state = QUECTEL_BT_ACPT_CONN_RESP;
      break;
    case QUECTEL_BT_ACPT_CONN_RESP:
         if(sim908->cmd.doneState==1){
            if(strstr((char*)sim908->cmd.repInfo,"+QBTACPT: 1")){
             connected_ID[0]=sim908->cmd.repInfo[12];
              sim908->cmd.got=1;
              sim908->cmd.ready=0;
              sim908->state=QUECTEL_BT_LISTEN;
              sim908->cmd.nbrTry = 0;
            }else{       
            sim908->state=QUECTEL_BT_ACPT_CONN;
            sim908->cmd.ready=0;
          }          
         }
         
         if(sim908->cmd.doneState==2){
            sim908->state=QUECTEL_BT_ACPT_CONN;
            sim908->cmd.ready=0;
        }
     break;
     
  }
  
}
   

void SIM908_debloque(sim908_struct*sim908){

  switch(sim908->debState){
  case DEB_SEND:
    sim908_send(sim908,"AT",1000);
    sim908->debState=DEB_RECEIVE;
    break;
    
  case DEB_RECEIVE:
    if(sim908->cmd.done==2){
      sim908->cmd.ready=0;
      sim908->debState=DEB_SEND;
    }
    if(sim908->cmd.doneState==1){
      if(strstr((char*)sim908->cmd.repState,"ERROR")){
        sim908->cmd.ready=0;
        sim908->debState=DEB_SEND;
      }
      if(strstr((char*)sim908->cmd.repState,ch_OK)){
        sim908->cmd.ready=0;
        sim908->cmd.got=1;
        sim908->cmd.doneState=0;
        sim908->debState=DEB_SEND;
        return;
      }
    }

    break;
  }
}

void sim908_init(sim908_struct* sim908){
  sim908->state=SIM908_INIT;//SIM908_SETUP_SEND;
  
  sim908->nbrTrySaveConfigNet=0;
  sim908->nbrTrySaveConfigFtp=0;
  sim908->download_state=SIM908_DOWNLOAD_SEND1;
  sim908->saveState=SIM908_SAVE_NET_SEND;
  sim908->setup_cursor=0;
  sim908->ftp_cursor=0;
  sim908->tryNumberMax=0;
  sim908->debState=DEB_SEND;
  cmd_init(&sim908->cmd);
  sim908->nbrTrySimInit=0;
  sim908_setFtpConfig(sim908);
}

void sim908_send(sim908_struct * sim908,char * data, uint32_t time){
  if(!sim908->cmd.ready){
   sim908->cmd.ready=1;
   strcpy(sim908->cmd.str,data);
   sim908->cmd.time_out=time;
   sim908->cmd.time=time;
  }
}

void sim908DecDelay(sim908_struct* sim908){
   if(sim908->cmd.time!=0)
    sim908->cmd.time--;
   if(sim908->timeout)
     sim908->timeout--;
}

//----------------------------------------------SERVER SETUP------------//
void sim908_setFtpConfig(sim908_struct* sim908){
  char chaine[64];
  SIM908_SetupInit();
  /*SIM908_setSupport("AT+QICSGP=1\r\n");   //AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"
  //apn
  strcpy(chaine,"AT+QICSGP=1,\""); //"AT+SAPBR=3,1,\"APN\",\""
  strcat(chaine,sim908->ftp.apn);
  strcat(chaine,"\"\r\n");
  SIM908_setApn(chaine);
  
  //apnuser
  strcpy(chaine,"AT+QICSGP=1,");//"AT+SAPBR=3,1,\"USER\","

  if(!strlen(sim908->ftp.apnUser)){
    strcpy(chaine,"AT+SAPBR=3,1,\"USER\",\"\"");
  }else{
  strcat(chaine,sim908->ftp.apnUser);
  }
  strcat(chaine,"\r\n");

  SIM908_setApnUser(chaine);
  ////// apnpwd
  strcpy(chaine,"AT+SAPBR=3,1,\"PWD\",");

  if(!strlen(sim908->ftp.apnPasswd)){
    strcpy(chaine,"AT+SAPBR=3,1,\"PWD\",\"\"");
  }else{
  strcat(chaine,sim908->ftp.apnPasswd);
  }

  strcat(chaine,"\r\n");

  SIM908_setApnPasswd(chaine); */
  //apn
  strcpy(chaine,"AT+QICSGP=1,\""); 
  strcat(chaine,sim908->ftp.apn);
  //strcat(chaine,"\"");
  strcat(chaine,"\",");

//apnuser
  if(!strlen(sim908->ftp.apnUser)){
    strcat(chaine,"");
  }else{
    strcat(chaine,sim908->ftp.apnUser);
  }
   strcat(chaine,",");

////// apnpwd
  if(!strlen(sim908->ftp.apnPasswd)){
    strcat(chaine,"");
  }else{
  strcat(chaine,sim908->ftp.apnPasswd);
  }
  //strcat(chaine,"\r\n");
    //strcat(chaine,"\"");*/
 SIM908_setSupport(chaine);
  
 /*  //serv
  strcpy(chaine,"AT+FTPSERV=\"");
  strcat(chaine,sim908->ftp.url);
  strcat(chaine,"\"\r\n");

  SIM908_setServAdr(chaine);
  //port
  strcpy(chaine,"AT+FTPPORT=");
  strcat(chaine,sim908->ftp.port);
  strcat(chaine,"\r\n");
  SIM908_setPort(chaine); */
  //serv
  strcpy(chaine,"AT+QFTPOPEN=\"");
  strcat(chaine,sim908->ftp.url);
  strcat(chaine,"\",");
  
  //port
  strcat(chaine,sim908->ftp.port);
  //strcat(chaine,"");
  //strcat(chaine,"\r\n");
  SIM908_setServAdrandport(chaine);
  
  //username
  strcpy(chaine,"AT+QFTPUSER=\"");  //AT+FTPUN
  strcat(chaine,sim908->ftp.username);
  //strcat(chaine,"\"");
  strcat(chaine,"\"");
  SIM908_setFtpLogin(chaine);
  //passwd
  strcpy(chaine,"AT+QFTPPASS=\""); //AT+FTPPW
  strcat(chaine,sim908->ftp.passwd);
  //strcat(chaine,"\"");
  strcat(chaine,"\"");
  SIM908_setFtpPasswd(chaine);
  //filename
  strcpy(chaine,"AT+QFTPGET=\""); //AT+FTPGETNAME=
  strcat(chaine,sim908->ftp.filename);
  strcat(chaine,"\",");
  strcat(chaine,sim908->ftp.filesize);
  //strcat(chaine,"");
  //strcat(chaine,"\r\n");
  SIM908_setFileNameandsize(chaine); 
  //path
  strcpy(chaine,"AT+QFTPPATH=\""); //AT+FTPGETPATH
  strcat(chaine,sim908->ftp.path);
  //strcat(chaine,"\"");
  strcat(chaine,"\"");
  SIM908_setFilePath(chaine);
  //Configuration_l'Emplacement_file_telecharger_depuis_l'FTP vers la RAM du module GSM
  //strcpy(chaine,"AT+QFTPCFG=4,\"/RAM/dwl.txt\"");
  strcpy(chaine,"AT+QFTPCFG=4,\"/RAM/dwl.txt\"");
  SIM908_setFTPConfEmplfile(chaine);  
}

void sim908_loadFtpConfig(sim908_struct* sim908){
 char tab[256],tab1[64];
 uint8_t i,i1=0;
 //from profile
/* strcpy(sim908->ftp.apn,sim908->pf.pc.apn);
 strcpy(sim908->ftp.apnUser,sim908->pf.pc.user);
 strcpy(sim908->ftp.apnPasswd,sim908->pf.pc.pass);*/
 
 //frome flash
 //"ip,port,username,paswd,path,filename"
 //URL_LOCATION
  memcpy(tab,(uint32_t*)URL_LOCATION,sizeof(tab));
  uint8_t counter_test=0;
  for(i=0;i<strlen(tab)+1&&i<256;i++){
    if(tab[i]==','||tab[i]=='\0'){
      tab1[i1]='\0';
      counter_test++;
      i1=0;
      if(counter_test==1){
        strcpy(sim908->ftp.url,tab1);
      }
      if(counter_test==2){
        strcpy(sim908->ftp.port,tab1);
      }
      if(counter_test==3){
        strcpy(sim908->ftp.username,tab1); 
      }
      if(counter_test==4){
        strcpy(sim908->ftp.passwd,tab1);
      }
      if(counter_test==5){
        strcpy(sim908->ftp.path,tab1);
      }
      if(counter_test==6){
        strcpy(sim908->ftp.filename,tab1);
      }
      if(counter_test==7){
        strcpy(sim908->ftp.apn,tab1);
      }
      if(counter_test==8){
        strcpy(sim908->ftp.apnUser,tab1);
      }
      if(counter_test==9){
        strcpy(sim908->ftp.apnPasswd,tab1);
      }
      if(counter_test==10){
        strcpy(sim908->ftp.filesize,tab1);
      }
    }else{
      tab1[i1++]=tab[i];
    }
  }
   //strcpy(sim908->ftp.filename,"LMF2-P_NTFU-1.15.0.0.hex");  //test.txt
   //strcpy(sim908->ftp.filesize,"90636"); //15
}
//--------------------------------------------------------------------------//
void SIM908_SetupInit(){
    strcpy(SIM908_SETUP_TAB[0],"AT"); //+QFTPCLOSE
    /*strcpy(SIM908_SETUP_TAB[1],"AT\r\n");         //+QICSGP=0
    strcpy(SIM908_SETUP_TAB[2],"AT\r\n");  //+CGATT=1
    strcpy(SIM908_SETUP_TAB[3],"AT\r\n"); //+CGREG=1
    strcpy(SIM908_SETUP_TAB[4],"AT\r\n");
    strcpy(SIM908_SETUP_TAB[5],"AT\r\n"); //+qifgcnt=0  //AT+SAPBR=1,1
    strcpy(SIM908_SETUP_TAB[6],"AT\r\n");
    strcpy(SIM908_SETUP_TAB[7],"AT\r\n");
    strcpy(SIM908_SETUP_TAB[8],"AT\r\n");
    strcpy(SIM908_SETUP_TAB[9],"AT\r\n"); */ //apn  internet.tn
    
    strcpy(SIM908_FTPCONF_TAB[0],"at+qifgcnt=0"); //+qifgcnt=0
                                  
   
}
void SIM908_setSupport(char *str){
  strcpy(SIM908_FTPCONF_TAB[GPRS_and_APN],str);
}
/*void SIM908_setApn(char *str){
  strcpy(SIM908_SETUP_TAB[APN],str);
}
void SIM908_setApnUser(char *str){
  strcpy(SIM908_SETUP_TAB[APN_USER],str);
}
void SIM908_setApnPasswd(char *str){
  strcpy(SIM908_SETUP_TAB[APN_PASSWD],str);
}*/
void SIM908_setServAdrandport(char *str){
  strcpy(SIM908_FTPCONF_TAB[SERV_ADR_and_PORT],str);
}
/*void SIM908_setPort(char *str){
  strcpy(SIM908_FTPCONF_TAB[PORT],str);
}*/
void SIM908_setFtpLogin(char *str){
  strcpy(SIM908_FTPCONF_TAB[FTP_LOGIN],str);
}
void SIM908_setFtpPasswd(char*str){
  strcpy(SIM908_FTPCONF_TAB[FTP_PASSWD],str);
}
void SIM908_setFileNameandsize(char *str){
  strcpy(SIM908_FTPCONF_TAB[FILE_NAME_and_Size_file],str);
}
void SIM908_setFilePath(char *str){
   strcpy(SIM908_FTPCONF_TAB[FILE_PATH],str);
}
void SIM908_setFTPConfEmplfile(char *str){
   strcpy(SIM908_FTPCONF_TAB[FTP_Conf_Empl_file],str);
}