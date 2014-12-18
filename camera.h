   #include <mega128.h> 
   #include <stdio.h>
   #include <alcd.h>
  
   #include <delay.h>
   #include <string.h>
   #include <stdlib.h> 
   #define ZERO 0x00
   #define led PORTC.0  
   
    void seta_imagem (void);
    void tx_dado ( flash char *data);
    void le_tamanho (void);
    void limpa_rx (void); 
    void le_foto(void);
    char  ver_fim (void);
    void salva_sd (char tam);
    void rx_serial (unsigned char dado_n);    
    
   extern char text1[33]; 
  char  rx_buffer[64];
  char  matriz[10];
  volatile char inde1; 
 // volatile unsigned char end_hight, end_low;   
  volatile unsigned int tamanho;
  long int i=0x0000;
  volatile unsigned char MH, ML;
  volatile char flg;
   
   
  flash char sizem[]= {9,'V', 0, '1', 5, 4, 1, 0, 0x19, 0};    
  flash char size_rx[] ={'v', 0, '1', 0, 0}; 
  flash char take[] ={5, 'V', 0, '6', 1, 0,}; 
  flash char take_rx[] ={'v', 0, '6', 0, 0,};    
  flash char read_f[] ={5,'V', 0, '4', 1, 0,};   
  flash char stop[] ={5, 'V', 0, '6', 1, 3,}; 
  flash char stop_rx[] ={'v', 0, '6', 0, 0,};   
//  flash char baud [] ={7, 0x56, 0, 0x24, 3, 01, 0x2A, 0xF2 }; 
//  flash char reset [] ={4, 0x56, 0, 0x26,0};

 
//###################################
//     salva do sdcard
//     save nbyte=tam in sdcard
//###################################
void salva_sd (char tam) 
 {
        char x,i;
         x=5;
         for(i=0;i<tam;i++,x++)
             {
              text1[i]=rx_buffer[x];
             }  
         escreve();        
  }      


 //###################################
//      ver fim do arquivo
//    check  end of file
//###################################
  
       char  ver_fim (void)
         {
          volatile char x,retorno;
          retorno=1;
          rx_serial (42);    // bytes para rx
          x=0;              // inico texto
                       
          do 
            {
             if((rx_buffer[x+6]==0xFF)&&(rx_buffer[x+8]==0xD9)&&(rx_buffer[x+9]==0x76))
               {
               retorno=0; 
               salva_sd(x+4);
               fecha();
               return retorno; 
               }
             else
             x++;             

            }  
            while(x!=37); 
            salva_sd(33);
            return retorno;           
 }          
          
 //###########################################
// USART0 Receiver interrupt service routine
//###########################################

void rx_serial (unsigned char dado_n)
{
  volatile unsigned char datax;

   limpa_rx();
    
   do
    {
    datax=getchar();                 // LED DADO DA SERIAL 
    rx_buffer[inde1] = datax;     // guarda dado na memoria
    inde1++;                      // incremanta indexador de endereço
    }
    while(inde1!=dado_n);
} 

 //############################
  //  seta tamanho da foto 
  // set size
  //############################  
  
 void seta_imagem (void)
 {
    
     tx_dado(sizem);
     rx_serial (5);
     if(strstrf(rx_buffer,size_rx))
       {
        lcd_gotoxy(0,0);
        lcd_putsf ("SIZE IMAGE      ");
        lcd_gotoxy(0,1); 
        lcd_putsf ("640 X 480       ");
       }            
     else
       {
        lcd_gotoxy(0,0);
        lcd_putsf ("CAMERA NAO RX");
       }
  }


  
  //############################
  //  limpa buffer rx 
  // clear buffer_rx serial
  //############################   
  void limpa_rx (void)
  {
   char i;
   for(i=0;i<64;i++)
   {
    rx_buffer[i]=0; 
   }; 
   inde1=0;
   
   for(i=0;i<33;i++)
   {
    text1[i]=0; 
   };   
  } 
    
   //############################
  //    tira foto  
  // take picture
  //############################ 
  
   void tira_foto (void)
 {
    tx_dado(take);
    rx_serial (5);
    if(strstrf(rx_buffer,take_rx))
      {
       lcd_gotoxy(0,0); 
       lcd_putsf ("TAKE A PICTURE   ");
       lcd_gotoxy(0,1);
       lcd_putsf ("WAITHING......   ");
      } 
    else
       {
        lcd_gotoxy(0,0);
        lcd_putsf ("ERROR TAKE    ");
       }

  }   
  
  
  //############################
  //     envia dados
  // send msg to serial
  //############################
   void tx_dado ( flash char *data)
   {

     volatile char i,x; 
     x=data[0];
     for (i=1;i<x+1;i++)
     {  
     putchar(data[i]);
     };
   }
   
  
 //############################
  //  le tamanho arquivo 
  //  read file size
  //############################  
  
  void le_tamanho (void)
 {  
    limpa_rx();
    tx_dado(read_f);
    rx_serial (9); 
    tamanho = (rx_buffer[7]<<8)+rx_buffer[8];
    lcd_gotoxy(0,0); 
    lcd_putsf ("READING PICTURE ");
    lcd_gotoxy(0,1);
    ltoa(tamanho,matriz);
    lcd_puts(matriz);
    lcd_putsf (" = BYTES   ");
         
  }   
  
//##########################
//  LE FOTO DA CAMERA
// read bytes of the picture
//###########################   
   
 void le_foto(void)
{
    MH=i/0x100;
    ML=i%0x100;
    putchar(0x56);
    putchar(ZERO);
    putchar(0x32);
    putchar(0x0c);
    putchar(ZERO);
    putchar(0x0a);
    putchar(ZERO);
    putchar(ZERO);
    putchar(MH);
    putchar(ML);
    putchar(ZERO);
    putchar(ZERO);
    putchar(ZERO);
    putchar(0x20);
    putchar(ZERO);
    putchar(0x0a);
    i+=0x20;
}

//##########################
//  para leitura de foto
// stop take picture
//###########################    
void stop_img (void)
 {
    
     tx_dado(stop);
     rx_serial (5);
     if(strstrf(rx_buffer,stop_rx))
       {
        lcd_gotoxy(0,0);
        lcd_putsf ("STOPPED READING");
        lcd_gotoxy(0,1); 
        lcd_putsf ("SAVED IN SDCARD");
       }            
     else
       {
        lcd_gotoxy(0,0);
        lcd_putsf ("CAMERA NAO RX");
       }
  }
 