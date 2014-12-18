/*****************************************************

*****************************************************/

#include <card.h>
#include <camera.h>


void main(void)
{

TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
TCNT1H=0xD8;
TCNT1L=0xF0;
TIMSK=1<<TOIE1;
PORTC=0x00;
DDRC=0xFF;
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL0) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=0x00;
UBRR0L=0x0C;
inde1=0;

#asm("sei")
lcd_init(16);

lcd_putsf ("CAMERA TTL AND  ");
lcd_gotoxy(0,1);
lcd_putsf ("SDCARD ATMEGA128");             
delay_ms(1500);
lcd_clear();
lcd_gotoxy(0,0);
lcd_putsf ("PREPARE SDCARD ");
delay_ms(1500);
monta();
delay_ms(1500);
cria_arquivo();
delay_ms(1000);
lcd_gotoxy(0,0);    
seta_imagem();
delay_ms(2000);
tira_foto();
delay_ms(4000);
le_tamanho();
delay_ms(2000);
flg=1;


 while (flg)
 
       {   
        le_foto();
        flg=ver_fim();
        if(i>=tamanho+32)
        {
         flg=0;
         fecha();       // close file
         }
       }; 
      
       stop_img(); 
   while (1);
}
