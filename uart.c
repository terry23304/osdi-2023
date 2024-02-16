#include "uart.h"
#include "gpio.h"

void uart_init()
{
    // Enable mini UART. Then mini UART register can be accessed.
    *AUX_ENABLES |= 1;
    // Disable transmitter and receiver during configuration.
    *AUX_MU_CNTL_REG = 0;
    // Disable interrupt because currently you don’t need interrupt.
    *AUX_MU_IER_REG = 0;
    // Set the data size to 8 bit.
    *AUX_MU_LCR_REG = 3;
    // Don’t need auto flow control.
    *AUX_MU_MCR_REG = 0;
    // Set AUX_MU_BAUD to 270. Set baud rate to 115200
    // After booting, the system clock is 250 MHz.
    *AUX_MU_BAUD_REG = 270;
    // No FIFO.
    *AUX_MU_IIR_REG = 0xc6;
    // Enable the transmitter and receiver.

    // Map UART1 (mini UART) to GPIO pins
    // TXD1: GPIO14, RXD1: GPIO15 (P102)
    unsigned int r = *GPFSEL1;
    // Clear bits 12-14 (GPIO14) and 15-17 (GPIO15) (P92)
    r &= ~((7 << 12) | (7 << 15));
    // Set GPIO14 15 to ALT5 (P102)
    r |= (2 << 12) | (2 << 15);
    *GPFSEL1 = r;
    
    *GPPUD = 0;            // enable pins 14 and 15
    r = 150; while (r--) {asm volatile("nop");}
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    r = 150; while (r--) {asm volatile("nop");}
    *GPPUDCLK0 = 0;        // flush GPIO setup

    *AUX_MU_CNTL_REG = 3;
}

// Check AUX_MU_LSR_REG’s data ready field.
// If set, read from AUX_MU_IO_REG
char read_uart_char()
{
    char c;
    while(1) {
        if(*AUX_MU_LSR_REG & 0x01) {
            c = (char)(*AUX_MU_IO_REG);
            if (c == '\r')
                c = '\n';
            return c;
        }
    }
}

// Check AUX_MU_LSR_REG’s Transmitter empty field.
// If set, write to AUX_MU_IO_REG
void write_uart(unsigned int c)
{
    while(!(*AUX_MU_LSR_REG & 0x20)) {}
    *AUX_MU_IO_REG = c;
}

void write_uart_string(char *str)
{
    while(*str != '\0') {
        if (*str == '\n')
            write_uart('\r');
        write_uart(*str++);
    }
}

void write_uart_hex(unsigned int num)
{
    int i;
    char str[10];
    for (i = 0; i < 8; i++) {
        str[7 - i] = num & 0xf;
        if (str[7 - i] < 10)
            str[7 - i] += '0';
        else
            str[7 - i] += 'a' - 10;
        num >>= 4;
    }
    str[8] = '\n';
    str[9] = '\0';
    write_uart_string("0x");
    write_uart_string(str);
}