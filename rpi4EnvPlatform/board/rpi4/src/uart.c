#include "rpi4hw.h"
#include "uart.h"

char read_reg32(long reg)
{
    return *(volatile unsigned int *)reg;
}

void write_reg32(long reg, unsigned int val)
{
    *(volatile unsigned int *)reg = val;
}

void delay(int steps)
{
    while(steps--) { asm volatile("nop"); }
}

void uart_init()
{
  unsigned int selector;

    selector = read_reg32(GPFSEL1); // get gpio control register value
    selector &= ~(7<<12);                   // clean gpio14
	  selector |= 4<<12;                      // set alt0 for gpio14
	  selector &= ~(7<<15);                   // clean gpio15
	  selector |= 4<<15;                      // set alt0 for gpio15
    write_reg32(GPFSEL1, selector);

    // config GPPUD and GPPUDCLK0 to config gpio, maybe you cam get the answer
    /*
    The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on
    the respective GPIO pins. These registers must be used in conjunction with the GPPUD
    register to effect GPIO Pull-up/down changes. The following sequence of events is
    required:
    1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
    to remove the current Pull-up/down)
    2. Wait 150 cycles – this provides the required set-up time for the control signal
    3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
    modify – NOTE only the pads which receive a clock will be modified, all others will
    retain their previous state.
    4. Wait 150 cycles – this provides the required hold time for the control signal
    5. Write to GPPUD to remove the control signal
    6. Write to GPPUDCLK0/1 to remove the clock
    */
    write_reg32(GPPUD,0);
    delay(150);
    write_reg32(GPPUDCLK0,(1<<14)|(1<<15));
    delay(150);
    write_reg32(GPPUDCLK0,0);

    write_reg32(UART0_CR, 0); //disable uart0
    write_reg32(UART0_LCRH, 0x60); // set to character mode, 8 bits word len and disable Parity
    write_reg32(UART0_IBRD, 0x1a);
    write_reg32(UART0_FBRD, 0x3); // set burdrate to 115200
    write_reg32(UART0_DMACR, 0x00); //disable dma
    write_reg32(UART0_ITCR, 0x0); 
    write_reg32(UART0_ITOP, 0x0); 
    write_reg32(UART0_TDR, 0x0); 
    write_reg32(UART0_CR, 0x301); //enable uart0
}

void uart_putchar(char c)
{
    if(c == '\n')   
        uart_putchar('\r');
    while(1){
        if(read_reg32(UART0_FR)&0x80 || !(read_reg32(UART0_FR)&0x20))
            break;
    }
    //if(c == '\n')   c = '\r';
    write_reg32(UART0_DR, c);
}

char uart_getchar()
{
    while (1){
        if(read_reg32(UART0_FR)&0x40)
            break;
    }
    return(read_reg32(UART0_DR)&0xff);
}

