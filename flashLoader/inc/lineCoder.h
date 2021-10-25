#include "stm32g4xx.h"
#include <string.h>
#include "utiles.h"



#define DEB_FLASH 0x8000000

#define nbr_word_max 4//4

//#ifndef DEBUG
//  #define DEBUG
//#endif



typedef struct {
  uint8_t length;
  uint32_t adresse;
  uint8_t type;
  uint32_t data [nbr_word_max];
 // uint64_t data_1 [2];
  uint8_t sum ; //.........
  uint8_t valide;
}lineCoder;

void fflash(char * t, uint8_t taille);
void fflash_data(uint32_t *t, uint8_t taille_data);
int rech_tab(uint32_t *tab,uint8_t taille,uint32_t elmt);
uint8_t LC_build(lineCoder  * lc,char * line);
void LC_save(lineCoder  * lc,uint32_t offset);
void flash_saveWord(uint32_t data,uint32_t pointer);
void supp_flash(uint32_t adr_debut , int nbr_of_page);
