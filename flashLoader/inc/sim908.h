#include "stm32g4xx.h"
//#include <string.h>
#include "flashLoader.h"


#define SIM908_SETUP_NBR 1  //10
#define SIM908_FTPCONF_NBR 8 //6
//***********************IP_setup
// #define GPRS 4
#define GPRS_and_APN 1 //9           
/*  #define APN 5
    #define APN_USER 8
    #define APN_PASSWD 7  */
/*#define SERV_ADR 0
#define PORT 1
#define FTP_LOGIN 2
#define FTP_PASSWD 3
#define FILE_PATH 4
#define FILE_NAME 5*/

#define FTP_LOGIN 2                 //0
#define FTP_PASSWD 3                //1
#define SERV_ADR_and_PORT 4         //2
#define FTP_Conf_Empl_file 5        //3
#define FILE_PATH 6                 //4
#define FILE_NAME_and_Size_file 7   //5



typedef enum {CMD_IDLE,CMD_WAIT}cmd_state;
typedef struct{
  cmd_state state;
  uint8_t ready;
  __IO uint8_t done;
  __IO uint8_t doneData;
  __IO uint8_t doneInfo;
  __IO uint8_t doneState;
  uint8_t got;
  uint8_t nbrTry;
  char  str[64];
  __IO char repData[64];
  __IO char repInfo[32];
  __IO uint8_t repState[32];
  uint32_t time_out;
  __IO uint32_t time;
  __IO uint8_t buffer[64];
  __IO uint8_t cursor_buff;
  __IO char connectedID[10];
}cmd_struct;

typedef struct{
  char url[32];
char port[8];
char apn[32];
char apnUser[32];
char apnPasswd[32];
char username[32];
char passwd[32];
char filename[45];
char filesize[8];
char path[32];
}ftp_struct;

typedef enum {
//---------
QUECTEL_BT_STATE,QUECTEL_BT_STATE_RESP,QUECTEL_BT_OFF,QUECTEL_BT_OFF_RESP,QUECTEL_BT_PAIR_CNF,QUECTEL_BT_PAIR_CNF_RESP,QUECTEL_BT_POWER,QUECTEL_BT_POWER_RESP,
QUECTEL_BT_LISTEN,QUECTEL_BT_ACPT_PAIR,QUECTEL_BT_ACPT_PAIR_RESP,QUECTEL_BT_ACPT_CONN,QUECTEL_BT_ACPT_CONN_RESP,QUECTEL_BT_READ,
  QUECTEL_BT_READ_RESP,QUECTEL_BT_SEND,QUECTEL_BT_SEND_RESP,QUECTEL_BT_MESSAGE,QUECTEL_BT_MESSAGE_RESP,
SIM908_INIT,SIM908_WR_NET,SIM908_CHECK_NET,SIM908_INIT_RESP,SIM908_RESTART,SIM908_PROFILE_INIT,SIM908_FTP_INIT,
SIM908_SET_CONF,SIM908_SAVE_CONFIG,SIM908_FTP_CONNECT,SIM908_FIN
}sim908_state;
typedef enum {SIM908_DOWNLOAD_SEND1,SIM908_DOWNLOAD_SEND2,SIM908_DOWNLOAD_GET1,SIM908_DOWNLOAD_GET2,
SIM908_DOWNLOAD_SEND3,SIM908_DOWNLOAD_GET3,SIM908_DOWNLOAD_SEND_DEB_LIGNE,SIM908_DOWNLOAD_GET_DEB_LIGNE,
SIM908_DOWNLOAD_SEND_LEN_LIGNE,SIM908_DOWNLOAD_GET_LEN_LIGNE
}SIM908_FTP_CONNECT_STATE;
typedef enum {DEB_SEND,DEB_RECEIVE}deb_state;
typedef enum {SIM908_SAVE_NET_SEND,SIM908_SAVE_NET_RESP,SIM908_SAVE_FTP_SEND,
SIM908_SAVE_FTP_RESP,SIM908_SAVE_NET_CLOSE_RESP,SIM908_SAVE_NET_CLOSE
}SIM908_SAVE_STATE;
typedef struct{
 //char tab[128];//********************
  cmd_struct cmd;
  sim908_state state;
  SIM908_FTP_CONNECT_STATE download_state;
  SIM908_SAVE_STATE saveState;
  deb_state debState;
  ftp_struct ftp;
  uint8_t setup_cursor;
  uint8_t ftp_cursor;
  uint8_t nbrTrySimInit;
  uint8_t nbrTrySaveConfigNet;
  uint8_t nbrTrySaveConfigFtp;
  uint8_t nbrTryFtpConnect;  
  uint8_t tryNumberMax;  
  uint8_t nbrTryDownload;
  uint32_t lenLineCourant;
  __IO uint32_t timeout;
  uint8_t nbrReset;
}sim908_struct;


void sim908_send(sim908_struct*sim908,char * data, uint32_t time);
void SIM908_setSupport(char *str);
void SIM908_setApn(char *str);
void SIM908_setServAdrandport(char *str);
void SIM908_setPort(char *str);
void SIM908_setFtpLogin(char*str);
void SIM908_setFtpPasswd(char*str);
void SIM908_setFileNameandsize(char *str);
void SIM908_setFilePath(char *str);
void SIM908_setFTPConfEmplfile(char *str);
void SIM908_setApnUser(char *str);
void SIM908_setApnPasswd(char *str);
void SIM908_SetupInit();
void SIM908_debloque(sim908_struct*sim908);
void gpio_conf();
void SIM908_Usart_Init();
void SIM908_Nvic_Config();
void usart_send(char * data);
void sim908_receiveChar(sim908_struct * sim908,uint8_t c);
void cmd_routine(cmd_struct * cmd);
void cmd_init(cmd_struct*cmd);
void sim908_init(sim908_struct* sim908);
void sim908DecDelay(sim908_struct* sim908);
void test_routine(sim908_struct* sim908,data_flash_struct *data_flash);
void sim908_setFtpConfig(sim908_struct* sim908);//struct ----> tab
void sim908_loadFtpConfig(sim908_struct* sim908);//flash----->struct
void bkp_init();


