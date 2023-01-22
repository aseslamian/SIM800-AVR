#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>
#include <stdio.h>
#define ctrl_z 0x1a
#define enter 0x0d

interrupt [EXT_INT0] void ext_int0_isr(void){
lcd_clear();
            lcd_putsf("SEND SMS ...");
            printf("AT");
            putchar(enter);
            delay_ms(1000);
            lcd_gotoxy(0,0);
          
            printf("AT+CMGF=1");
            putchar(enter);
            delay_ms(1000);
            printf("AT+CMGS=\"09XXXXXXXXX\"");
            putchar(enter);
            delay_ms(1000);
           printf ("TEST HELLO");
            putchar(ctrl_z);
            delay_ms(500); 
            lcd_clear();
            lcd_putsf("SMS Send SIM800");
            delay_ms(5000);
}

void main(void)
{
//usart
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;

//interupt
GICR|=0x40;
MCUCR=0x03;
GIFR=0x40;
#asm("sei");

//LCD initial
lcd_init(16);
lcd_clear();
lcd_putsf("READY");
lcd_gotoxy(0,1);
delay_ms(1000);

while (1)
      {
      lcd_clear();
      lcd_gotoxy(0,0);
      lcd_puts("Start");      
               
             
      
 }          
}
