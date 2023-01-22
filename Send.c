#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<DOR)
#define RX_BUFFER_SIZE 8
#define enter 0x0d
#define ctrl_z 0x1a
char rx_buffer[RX_BUFFER_SIZE];
char BUFF[100],i=0,j=0,R=1;
unsigned char k=0;
#if RX_BUFFER_SIZE <= 256
unsigned char rx_wr_index=0,rx_rd_index=0;
#else
unsigned int rx_wr_index=0,rx_rd_index=0;
#endif

#if RX_BUFFER_SIZE < 256
unsigned char rx_counter=0;
#else
unsigned int rx_counter=0;
#endif

// This flag is set on USART Receiver buffer overflow
bit rx_buffer_overflow;

// USART Receiver interrupt service routine
interrupt [USART_RXC] void usart_rx_isr(void)
{
char status,data;
status=UCSRA;
data=UDR;
BUFF[i]=data;
i++;

if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
   {
   rx_buffer[rx_wr_index++]=data;
#if RX_BUFFER_SIZE == 256
   // special case for receiver buffer size=256
   if (++rx_counter == 0) rx_buffer_overflow=1;
#else
   if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
   if (++rx_counter == RX_BUFFER_SIZE)
      {
      rx_counter=0;
      rx_buffer_overflow=1;
      }
#endif
   }
}

#ifndef _DEBUG_TERMINAL_IO_
// Get a character from the USART Receiver buffer
#define _ALTERNATE_GETCHAR_
#pragma used+
char getchar(void)
{
char data;
while (rx_counter==0);
data=rx_buffer[rx_rd_index++];
#if RX_BUFFER_SIZE != 256
if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
#endif
#asm("cli")
--rx_counter;
#asm("sei")
return data;
}
#pragma used-
#endif
#include <stdio.h>

void main(void)
{
PORTB=0xff;
DDRB=0x00;

lcd_clear();
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
UCSRC=(1<<URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCPOL);
UBRRH=0x00;
UBRRL=0x33;
lcd_init(16);
#asm("sei")
lcd_clear();
for(j=0;j<3;j++){
printf("AT");
putchar(enter);
delay_ms(1000);
}
for(j=0;j<2;j++){
printf("AT+CMGF=1");
putchar(enter);
delay_ms(1000);
}
UCSRB=(1<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
delay_ms(200);

lcd_clear();
while (1){
            R=1;

            if(k<1){
            printf("AT+CMGR=1");
            putchar(enter);
            k++;
            }
            lcd_gotoxy(0,0);
            lcd_puts(BUFF);
            if(PINB.1==0){
            #asm("cli");
            lcd_clear();
            lcd_putsf("CLEAR ALL");
            printf("AT+CMGDA=\"DEL ALL\"");
            putchar(enter);
            delay_ms(1000);
            i=0;
            lcd_clear(); 
            #asm("sei");
            lcd_putsf("DONE!");
            
            
            }
            while(PINB.0==0 && R==1){
            #asm("cli");
            delay_ms(1000);
            lcd_clear();
            lcd_putsf("SENDING...");    // This massage will be shown on LCD
            printf("AT");     // AT Command for acknowlagement
            putchar(enter);
            delay_ms(1000);
            lcd_gotoxy(0,0);
          
            printf("AT+CMGF=1");
            putchar(enter);
            delay_ms(1000);
            printf("AT+CMGS=\"09XXXXXXX\"");   // The massage will be sent to this number
            putchar(enter);
            delay_ms(1000);
            printf ("KING");    // you can change the massage text
            putchar(ctrl_z);
            delay_ms(500); 
            lcd_clear();
            lcd_putsf("DONE!");
            delay_ms(2000);
            R=0;
            #asm("sei");
            }
            
}     
}
