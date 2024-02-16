#include "lib.h"
#include "uart.h"
#include "mailbox.h"

void shell()
{
    char c;
    while(1) {
        write_uart_string("$ ");
        static char buf[100];
        int i = 0;
        while(1) {
            c = read_uart_char();
            write_uart(c);
            buf[i++] = c;
            if (c == '\n' || c == '\r')
                break;
        }
        buf[i] = '\0';

        if (strcmp(buf, "hello\n") == 0) {
            write_uart_string("Hello World!\n");
        } else if (strcmp(buf, "help\n") == 0) {
            write_uart_string("help     : print this menu\n");
            write_uart_string("hello    : print Hello World!\n");
            write_uart_string("reboot   : reboot the device\n");
        } else if (strcmp(buf, "info\n") == 0) {
            get_board_revision();
            get_arm_memory();
        } else {
            write_uart_string("Unknown command: ");
            write_uart_string(buf);
        }
    }
}