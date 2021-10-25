#include "lineCoder.h"
#include "stm32g4xx_hal_flash.h"
#include "flash.h"
//#include "stm32g4xx_hal_flash.h"
//extern char buff_2[4];
//*************vider le buffer.
//static void FLASH_Program_Fast(uint32_t Address, uint32_t DataAddress);
void fflash(char * t, uint8_t taille){
  uint8_t i;
  for(i=0;i<taille;i++){
    t[i]='\0';
  }
}

//*****************vider le tab data
void fflash_data(uint32_t *t, uint8_t taille_data){
  uint8_t i;
  for(i=0;i<taille_data;i++)
    t[i]=0;
}


//***********rech elmt dans tab(taille)
int rech_tab(uint32_t *tab,uint8_t taille,uint32_t elmt){
  int i;
  for(i=0;i<taille;i++){
    if(tab[i]==elmt)
      return i;
  }
  return -1;
}

//************remplir les champs d'une linge.
uint8_t LC_build(lineCoder  * lc,char * line){
  char buff_1[9],buff[9];
  uint8_t somme=0;
  char buff_somme[3];
  uint8_t cursor=1;
  
  /*char buff_2[16];
  strncpy(buff_2,&line[9],16);
  lc->data_1[0]=hex_from_string_1(buff_2);*/
//longueur des données.
  
  //strncpy(buff_2,&line[3],4);
  
  fflash(buff,9);
  strncpy(buff,&line[cursor],2);
  somme=hex_from_string(buff);
  lc->length=hex_from_string(buff);
  
  //adresse de 1er octet dans la linge
  cursor=3;
  fflash(buff,9);
  fflash(buff_somme,3);
  strncpy(buff,&line[cursor],4);
  buff_somme[0]=buff[0];
  buff_somme[1]=buff[1];
  somme=somme+hex_from_string(buff_somme);
  buff_somme[0]=buff[2];
  buff_somme[1]=buff[3];
  somme=somme+hex_from_string(buff_somme);
  lc->adresse=hex_from_string(buff);
  
  //defenir le type de linge
  cursor = 7;
  
  fflash(buff,9);
  strncpy(buff,&line[cursor],2);
  somme=somme+hex_from_string(buff);
  lc->type=hex_from_string(buff);
  
  //remplir les données dans data
  fflash_data(lc->data,nbr_word_max);
  cursor=9;
  uint8_t i,j=0;
  for(i=0;i< lc->length*2;i=i+8){
    uint8_t x=8;//longueure de mot (en octet)
    if((lc->length*2 - i) < 8)
     x=lc->length*2 - i;
     fflash(buff,9);
    strncpy(buff,&line[cursor+i],x);
   
    fflash(buff_1,9);//mise en forme du mot
    uint8_t l ;
    for(l=0;l<8;l+=2){
    buff_1[l+1]=buff[x-1-l];
    buff_1[l]=buff[x-2-l];
    buff_somme[0]=buff_1[l];
    buff_somme[1]=buff_1[l+1];
    somme=somme+hex_from_string(buff_somme);
     
    }
      
   lc->data[j]=hex_from_string(buff_1);
    //lc->data[j]=hex_from_string(buff_1);
    j++;
  }
  
  //somme de données
  cursor=9+(lc->length)*2;
  fflash(buff,9);
  strncpy(buff,&line[cursor],2);
  lc->sum=hex_from_string(buff);
  
  //vérification
  lc->valide=0;
  if(((somme + lc->sum)&0xff )== 0x00 )
  lc->valide =1 ;
  
   return lc->valide;
}


//*********************************** data ===> flash
void LC_save(lineCoder  * lc,uint32_t offset){
  uint32_t  pointer;
  //uint8_t i;
  pointer = lc->adresse + offset + DEB_FLASH;
   //pointer = 0x8010000+16;
   //FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
  /*uint64_t data[1]={0};
        data[0]=(lc->data[1]); // >> 24
        data[0]=(lc->data[0]|lc->data[1]);
        //data[0]=(lc->data[0]|lc->data[1]);*/
  /*uint64_t dataToSave[2]={0};
  dataToSave[0]=(((uint64_t)lc->data[0])&0x00000000FFFFFFFF)|(((uint64_t)lc->data[1]<<32)&0xFFFFFFFF00000000);
  dataToSave[1]=(((uint64_t)lc->data[2])&0x00000000FFFFFFFF)|(((uint64_t)lc->data[3]<<32)&0xFFFFFFFF00000000);
*/  
//HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,pointer,dataToSave);//FLASH_ProgramWord(pointer,lc->data[i]);
 // for(i=0;i<2;i++){//4  lc->length/8
  if(pointer==0x8010000){
   // uint64_t marker=0xBBBBBBBBBBBBBBBB;
    AJAJA_WriteXWord(pointer,1,(uint32_t *)lc->data);
  //AJAJA_WriteXWord(0x8010000,1,(uint32_t *)&marker);
  }
    AJAJA_WriteXWord(pointer,1,(uint32_t *)lc->data);
    AJAJA_WriteXWord(pointer+8,1,(uint32_t *)lc->data+2);
   // AJAJA_WriteXWord(pointer,1,(uint32_t *)lc->data[2]);
    /*HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,pointer,dataToSave[i]);//FLASH_ProgramWord(pointer,lc->data[i]);
   // FLASH_Program_Fast(pointer, lc->data);
    pointer+=8; //pointer+=4;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,pointer,dataToSave[i]);//FLASH_ProgramWord(pointer,lc->data[i]);
    pointer+=8;*/
    //}*/
  /*for(int i=0;i<16;i+=8){
     AJAJA_WriteXWord(pointer+i,1,(uint64_t *)(dataToSave+i));
  }*/
    
 
}


   //sim908_loadFtpConfig(&sim908);
/*void flash_saveWord(uint32_t data,uint32_t pointer){
   //FLASH_Unlock();
   //FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
   FLASH_ProgramWord(pointer,data);
   //FLASH_Lock();
}*/

//***************** supp flash
void supp_flash(uint32_t adr_debut , int nbr_of_page){
  int i,j=1000000;
  //if(adr_debut==0x8010000){
  
    for(i= 0; (i < nbr_of_page ); j--){
      
      if(j==0){
         FLASH_G4_ErasePage(((adr_debut + i* 0x800) -0x8000000)/0x800,1);//FLASH_ErasePage(adr_debut + i* 0x400); 
         j=10000000;
         i++;
      }
     /* j--
      while(j!=0){
       j==1000;*/
      }
     // while(machine->time!=0);
   // }
 // }
 /* else{
  //FLASH_G4_ErasePage(4,1);
  FLASH_G4_ErasePage(5,1);
  FLASH_G4_ErasePage(6,1);
  FLASH_G4_ErasePage(7,1);
  FLASH_G4_ErasePage(8,1);
  FLASH_G4_ErasePage(9,1);
  FLASH_G4_ErasePage(10,1);
  FLASH_G4_ErasePage(11,1);
  FLASH_G4_ErasePage(12,1);
  FLASH_G4_ErasePage(13,1);
  FLASH_G4_ErasePage(14,1);
  FLASH_G4_ErasePage(15,1);
  FLASH_G4_ErasePage(16,1);
  FLASH_G4_ErasePage(17,1);
  FLASH_G4_ErasePage(18,1);
  FLASH_G4_ErasePage(19,1);
  FLASH_G4_ErasePage(20,1);
  FLASH_G4_ErasePage(21,1);
  FLASH_G4_ErasePage(22,1);
  FLASH_G4_ErasePage(23,1);
  FLASH_G4_ErasePage(24,1);
  FLASH_G4_ErasePage(25,1);
  FLASH_G4_ErasePage(26,1);
  FLASH_G4_ErasePage(27,1);
  FLASH_G4_ErasePage(28,1);
  FLASH_G4_ErasePage(29,1);
  FLASH_G4_ErasePage(30,1);
  FLASH_G4_ErasePage(31,1);
  }*/
}

//**************************mise_a_jour
/*
uint8_t  mise_a_jour(){
char * USB_GetData();
  
  lineCoder lc;
  uint8_t tab_i=0,nbr_corrig=0,tab_n=0,tab_buff_i=0,tab_buff_n=0,nbr_max=0;
  uint32_t tab_verif[200],tab_buff[30]={0xffffffff};
  int k=-1;
 
    while (1) 
  {
    if(USB_getDataReady()){
      if(LC_build(&lc,USB_GetData()))
      if(lc.type == 0x00){
        LC_save(&lc,0);
        tab_verif[tab_i++]=lc.adresse;
      }
      if(lc.type==0x01)
     break;
    }
  }
  //printf("terminer"); 
  tab_n=tab_i;
   if(tab_n==0){return 1;}
  
   for(tab_i=0;tab_i<tab_n;tab_i++){
    if(tab_verif[tab_i+1]>tab_verif[tab_i]+0x10){
      tab_buff[tab_buff_i]=tab_verif[tab_i]+0x10;
      tab_buff_i++;
    }
  }
  tab_buff_n=tab_buff_i;
  
loop_while:
  nbr_max++;
  while (1) 
  {k=-1;
    if(USB_getDataReady()){
      if(LC_build(&lc,USB_GetData())){
        k = rech_tab(tab_buff,tab_buff_n,lc.adresse);
        if(lc.type == 0x00&&k!=-1){
        LC_save(&lc,0);
        tab_buff[k]=0xffffffff;
        nbr_corrig++;
        }
      }
      
      if(lc.type==0x01)
     break;
    }
    if(nbr_corrig==tab_buff_n)
      return 1;
    else if(nbr_max<6)
      goto loop_while ;
    return 0;
            
  }
  
 }
*/




