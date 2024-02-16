#include "uart.h"

int main()
{
    uart_init();
    write_uart_string("Hello, world!\n");
    shell();
}