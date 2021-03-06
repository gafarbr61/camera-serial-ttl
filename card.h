/******************************************************************************
If you are using the CodeVisionAVR Evaluation version, you may program into
the FLASH the precompiled SDCARD5.HEX file found in the
******************************************************************************/


#include <ff.h>
#include <stdio.h>
#include <alcd.h>
#include <delay.h>
#include <string.h>


void monta (void);
void escreve (void);
void fecha (void);

FRESULT res;
unsigned int nbytes;
FATFS fat;
FIL file;
char path[256]="0:/";
char arquivo[]="0:/pic.jpg";


char text1[33] ;

/* 100Hz timer interrupt generated by ATmega128 Timer1 overflow */
interrupt [TIM1_OVF] void timer_comp_isr(void)
{

TCNT1H=0xD8F0 >> 8;
TCNT1L=0xD8F0 & 0xff;
// Place your code here
disk_timerproc();
}

/* error message list */
flash char * flash error_msg[]=
{
"", /* not used */
"FR_DISK_ERR",
"FR_INT_ERR",
"FR_NOT_READY",
"FR_NO_FILE",
"FR_NO_PATH",
"FR_INVALID_NAME",
"FR_DENIED",
"FR_EXIST",
"FR_INVALID_OBJECT",
"FR_WRITE_PROTECTED",
"FR_INVALID_DRIVE",
"FR_NOT_ENABLED",
"FR_NO_FILESYSTEM",
"FR_MKFS_ABORTED",
"FR_TIMEOUT"
};



 //############################
 //  msg de erro sdcard 
 //############################ 

void error(FRESULT res)
{
if ((res>=FR_DISK_ERR) && (res<=FR_TIMEOUT))
   {
   printf("ERROR: %p\r\n",error_msg[res]); 
   lcd_gotoxy(0,0);
   lcd_putsf ("ERRO NO SDCARD ");
   } 
   
}

 //############################
 //       FECHA ARQUIVO 
 //       close file
 //############################ 
    void fecha (void)
   { 
     if ((res=f_close(&file))!=FR_OK)
      error(res);
   }

 //############################
 //         monta 
 // mount logical drive 0: 
 //############################   
/* mount logical drive 0: */

 void monta (void)
 {
if ((res=f_mount(0,&fat))!=FR_OK)
   error(res);
 else  
  { 
  lcd_gotoxy(0,0);
  lcd_putsf ("LOGICAL DRIVE 0 ");
  lcd_gotoxy(0,1);
  lcd_putsf ("MOUNTED OK      "); 
  }
f_unlink(path);
 }
 
 //############################
 //   cria arquivo 
 // create new file
 //############################

 void cria_arquivo (void)
{  
if ((res=f_open(&file,arquivo,FA_CREATE_ALWAYS | FA_WRITE))!=FR_OK)
    error(res);
else  
   {
   lcd_gotoxy(0,0);
   lcd_putsf (" ARQUIVO CRIADO ");
   lcd_gotoxy(0,1);
   lcd_putsf (" FILE = PIC.JPG ");   
   }     
   
   
} 

 //###################################
 //  escreve 32 bytes de text1 sdcard 
 //  writes 32 bytes sdcard
 //################################### 
 void escreve (void)
  { 
 if ((res=f_write(&file,text1,sizeof(text1)-1,&nbytes))==FR_OK)
   error(res); 
}



 


