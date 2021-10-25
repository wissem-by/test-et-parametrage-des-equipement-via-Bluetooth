#include "utiles.h"
/*******************************************************************************
* Function Name  : isDigit.
* Description    : return true if char is disgit
* Input          : c : tested Char
* Return         : 1-if digit 0-else
*******************************************************************************/
uint8_t isDigit(uint8_t c){
   if(c>=0x30&&c<=0x39)
       return 1;
   return 0;
}
/*******************************************************************************
* Function Name  : isDigit.
* Description    : return true if char is disgit
* Input          : c : tested Char
* Return         : 1-if digit 0-else
*******************************************************************************/
uint8_t isHex(uint8_t c){
   if((c>=0x30&&c<=0x39)||(c>='A'&&c<='F')||(c>='a'&&c<='f'))
       return 1;
   return 0;
}
/*******************************************************************************
* Function Name  : uint8_t_to_hex.
* Description    : Convert uint8_t table into HEX String.
* Input          :   Data: uint8_ts to convert
                    Length: Length to convert 
* Output         :   convData: Converted data
* Return         : None
*******************************************************************************/
void uint8_t_to_hex (char *Data,uint8_t length ,char *convData)
{
  uint8_t i=0;
  while (i<length){
     if ( Data[i] >= 0xA0)
        convData[(length-i-1)*2]=(Data[i]>>4) - 0xA + 'A';
     else
        convData[(length-i-1)*2]=(Data[i]>>4)  + '0';
     
     if ((Data[i]&0x0F )> 9)
        convData[(length-i-1)*2+1]=(Data[i]&0x0F) -0xA + 'A';
     else
        convData[(length-i-1)*2+1]=(Data[i]&0x0F) + '0';

     i++;
  }
}

void uint32_t_to_hex(uint32_t Data,char* convData){
   uint8_t i;
   for (i=0;i<4;i++){
       char temp [2];
       temp[0]=Data&0x000000ff;
       uint8_t_to_hex(&temp[0],1,&convData[(3-i)*2]);
       Data=Data>>8;
   }
   convData[8]=0;
}
void uint16_t_to_hex(uint16_t Data,char* convData){
   uint8_t i;
   for (i=0;i<2;i++){
       char temp [2];
       temp[0]=Data&0x00ff;
       uint8_t_to_hex(&temp[0],1,&convData[(1-i)*2]);
       Data=Data>>8;
   }
   convData[4]=0;
}
void char_to_hex(char Data,char* convData){
   uint8_t_to_hex(&Data,1,convData);
   convData[2]=0;
}
void remove_padding(char * Data,uint8_t padding){
   uint8_t i=0;
   while(Data[i]==padding){
       i++;
   }
   if(i>0)
       strcpy((char *)Data,(char *)&Data[0]+i-1);
}
uint8_t hex_from_char(uint8_t c){
   if(c>='A'&& c<='F')
       return c-'A'+10;
   else if(c>='a'&& c<='f')
       return c-'a'+10;
   else if(c>='0'&& c<='9')
       return c-'0';
   return 0;
}
uint32_t hex_from_string (char * Data){
   uint32_t returned=0;
   uint8_t i;
   for(i=0;i<8&&isHex(Data[i]);i++)
       returned=returned*16+hex_from_char(Data[i]);
   return returned;
}
/*uint64_t hex_from_string_1 (char * Data){
   uint64_t returned=0;
   uint8_t i;
   for(i=0;i<16&&isHex(Data[i]);i++)
       returned=returned*16+hex_from_char(Data[i]);
   return returned;
}*/

/*******************************************************************************
* Function Name  : signedAbsCompare.
* Description    : Comparaison en valeur absolue avec conservation du signe.
* Input          : Data1, Data2: int8_t to compare             
* Return         : 1-If Data1 > Data2 0- else
*******************************************************************************/
uint8_t signedAbsCompare (int8_t Data1 , int8_t Data2){
  if ((Data1 >=0)&&(Data2>=0) &&(Data2 < Data1) ||
      (Data1 < 0)&&(Data2< 0) &&(Data2 > Data1) ||
      (Data1 >=0)&&(Data2< 0) &&(Data2 * (-1) < Data1) ||
      (Data1 < 0)&&(Data2>=0) &&(Data2  < Data1* (-1))){
     return (1);
  }
  return (0);
}

/*******************************************************************************
* Function Name  : int8_t_to_string.
* Description    : Convertir un nombre sign? en chaine de charact?re.
* Input          : Data1, Data2: int8_t to compare             
* Return         : 1-If Data1 > Data2 0- else
*******************************************************************************/
void int8_t_to_string (int32_t Data,char * convData){
  uint8_t i=1,j;
  if (Data>=0){
     convData[0]='+';
  }else{
     convData[0]='-';
     Data*=(-1);
  }
  do{
     for (j=i;j>1;j--)
        convData[j]=convData[j-1];
     convData[1]=(Data%10)+'0';
     Data=Data/10;
     i++;
  }while (Data>0);
  convData[i]=0x00;
}

/*******************************************************************************
* Function Name  : int8_t_to_string.
* Description    : Convertir un nombre sign? en chaine de charact?re.
* Input          : Data1, Data2: int8_t to compare             
* Return         : 1-If Data1 > Data2 0- else
*******************************************************************************/
void uint8_t_to_string (uint32_t Data ,char * convData){
  uint8_t i=0,j;
  while (Data>=10){
     for (j=i;j>0;j--)
        convData[j]=convData[j-1];
     convData[0]=(Data%10)+'0';
     Data=Data/10;
     i++;
  }
  for (j=i;j>0;j--)
        convData[j]=convData[j-1];
  convData[0]=(Data%10)+'0';
  convData[i+1]=0x00;
}
/*******************************************************************************
* Function Name  : uint8_t_to_string_fixed.
* Description    : Convertir un nombre sign? en chaine de charact?re en longeur fixe.
* Input          :  uint32_t Data data to convert
                   uint8_t * convData data converted
                   uint8_t len fixed length
* Return         : None.
*******************************************************************************/
void uint8_t_to_string_fixed (uint32_t Data ,char * convData,uint8_t len,uint8_t padding){
  uint8_t i=0;
  while (Data&&i<len){
     convData[len-i-1]=(Data%10)+'0';
     Data=Data/10;
     i++;
  }
  while(len-i){
       convData[len-i-1]=padding;
       i++;
  }
  convData[len]=0x00;
}
/*******************************************************************************
* Function Name  : uint8_t_to_string.
* Description    : Convertir un nombre sign? en chaine de charact?re.
* Input          : Data1, Data2: int8_t to compare             
* Return         : 1-If Data1 > Data2 0- else
*******************************************************************************/
/*uint8_t* uint8_t_to_string (int32_t Data){
  uint8_t convData[13];
  uint8_t i=0,j;
  while (Data/10>0){
     for (j=0;j<i;j++)
        Data[j+1]=Data[j];
     convData[i]=Data%10+'0';
     Data=Data/10;
     i++;
  }
  convData[i]=0x00;
  return convData;
}*/
/*******************************************************************************
* Function Name  : string_to_uint16_t.
* Description    : Convertir une chaine de caractere en uint16_t.
* Input          : data : uint8_t* String            
* Return         : uint16_t converted
*******************************************************************************/
uint32_t string_to_uint32_t(char * Data){
  uint8_t i=0 ;
  uint32_t returned=0;
  while (Data[i]!='\0' && i<10 && Data[i]>='0' && Data[i]<='9'){
     returned =returned*10+(Data[i]-'0');
     i++;
  }
  return (uint32_t)returned;
}
/*******************************************************************************
* Function Name  : nextToken.
* Description    : retourn la position du prochain token.
* Input          :  data : uint8_t* String : chaine a d?composer
                   uint8_t   : uint8_t token : token choisis
                   uint8_t   : uint8_t currentPos : la position actuelle dans la chaine de charactere
* Return         : uint8_t nextpos : position du prochain token
                               0 if failure
*******************************************************************************/
uint16_t nextToken(char * Data,uint8_t token, uint16_t currentPos)
{
   uint16_t i=currentPos;
   while (i<512&&Data[i]){
       if (Data[i]==token )
           return i;
       i ++;
   }
   return 0;
}
/*******************************************************************************
* Function Name  : GetPos.
* Description    : retourn la niemme position d'un caractere dans la chaine de caractere.
* Input          :  data : uint8_t* String : chaine a d?composer
                   uint8_t   : uint8_t token : token choisis
                   uint8_t   : uint8_t currentPos : la position actuelle dans la chaine de charactere
* Return         : uint8_t nextpos : position du prochain token
                               0 if failure
*******************************************************************************/
uint8_t GetPos(char *buf,uint8_t n,uint8_t c){
unsigned char i=0,k=0;
while(i<255 && k<255 && buf[k])
{
if (buf[k]==c) {
i++;
if (i==n) return (k+1);
}
   k++;
}
return(0);
}

/*******************************************************************************
* Function Name  : displayTrame.
* Description    : format trame for display
* Input          : trame
* Return         : none
*******************************************************************************/
/*void displayTrame(uint16_t trameAdr){
    char Data[256];
    uint32_t_to_hex(trameAdr+0x08010000,Data);
    strcat((char *)Data," @ ");
    strncat((char *)Data,(char *)trameAdr+0x08010000,LENGTH_TRAME);
    strcat((char *)Data,"\n\r");
    if(Data[14]!=0xFF)
       DEBUG_Write(Data);
}*/
/*******************************************************************************
* Function Name  : IWDG_reload.
* Description    : update the wtchdog timer.
* Input          : uint8_t : nouveau timeout
* Output         : None.
* Return         : None.
*******************************************************************************/
/*void IWDG_reload () {
   IWDG_ReloadCounter();
}*/
/*******************************************************************************
* Function Name  : uppercase.
* Description    : update the wtchdog timer.
* Input          : uint8_t : nouveau timeout
* Output         : None.
* Return         : None.
*******************************************************************************/
/*void ToUpperCase( char * sPtr ){
while( *sPtr != '\0' ){
*sPtr = toupper( ( unsigned char ) *sPtr );
++sPtr;
}
}*/
/*******************************************************************************
* Function Name  : uppercase.
* Description    : update the wtchdog timer.
* Input          : uint8_t : nouveau timeout
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t removeCrLf(char * Data){
 uint8_t cnt=0;
 while (Data[cnt]=='\n'||Data[cnt]=='\r'){
   cnt++;
 }
 return cnt;
}
/*******************************************************************************
* Function Name  : uppercase.
* Description    : update the wtchdog timer.
* Input          : uint8_t : nouveau timeout
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t hex2dec (char hex[])
{
    uint8_t i = 0;
    uint8_t dec = 0;
    uint8_t temp = 0;

    while (hex[i] != '\0')
    {
        const int isHEXA = (hex[i] >= 'A') && (hex[i] <= 'F');
        const int ishexa = (hex[i] >= 'a') && (hex[i] <= 'f');
        const int isdigit = (hex[i] >= '0') && (hex[i] <= '9');

        if (ishexa)
        {
            temp = hex[i] - 'a' + 10;
        }
        else if (isHEXA)
        {
            temp = hex[i] - 'A' + 10;
        }
        else if (isdigit)
        {
            temp = hex[i] - '0';
        }
        dec = dec * 16 + temp;

        i++;
    }

    return dec;
}