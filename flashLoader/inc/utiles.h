#include "stm32g4xx.h"
#include <string.h>


uint8_t isDigit(uint8_t c);
uint8_t isHex(uint8_t c);
void uint8_t_to_hex (char *Data,uint8_t length ,char *convData);
void uint32_t_to_hex(uint32_t Data,char* convData);
void uint16_t_to_hex(uint16_t Data,char* convData);
void char_to_hex(char Data,char* convData);
void remove_padding(char * Data,uint8_t padding);
uint8_t hex_from_char(uint8_t c);
uint32_t hex_from_string (char * Data);
uint8_t signedAbsCompare (int8_t Data1 , int8_t Data2);
void int8_t_to_string (int32_t Data,char * convData);
void uint8_t_to_string (uint32_t Data ,char * convData);
void uint8_t_to_string_fixed (uint32_t Data ,char * convData,uint8_t len,uint8_t padding);
uint32_t string_to_uint32_t(char * Data);
uint8_t GetPos(char *buf,uint8_t n,uint8_t c);
//void ToUpperCase( char * sPtr );
uint8_t removeCrLf(char * Data);
uint8_t hex2dec (char hex[]);