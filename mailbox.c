#include "mailbox.h"
#include "uart.h"

volatile unsigned int __attribute__((aligned(16))) mailbox[36];

int mailbox_call(unsigned char channel)
{
    // Combine the message address (upper 28 bits) with channel number (channel 8 (CPU->GPU))
    unsigned int r = (((unsigned int)((unsigned long)&mailbox) & ~0xF) | (channel & 0xF));
    // Check if Mailbox 0 is full
    while (*MAILBOX_STATUS & MAILBOX_FULL) {}
    *MAILBOX_WRITE = r;
    while(1) {
        // Check if Mailbox 0 is empty
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) {}
        // Check if the value is the same as you wrote
        if (r == *MAILBOX_READ)
            return mailbox[1] == REQUEST_SUCCEED;
    }
}

void get_board_revision()
{
    mailbox[0] = 7 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_BOARD_REVISION; // tag identifier
    mailbox[3] = 4; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    // tags end
    mailbox[6] = END_TAG;
    if (mailbox_call(8)) {
        write_uart_string("Board revision is: ");
        write_uart_hex(mailbox[5]);
    } else
        write_uart_string("Failed to get board revision\n");
    
}

void get_arm_memory()
{
    mailbox[0] = 8 * 4; // buffer size in bytes
    mailbox[1] = REQUEST_CODE;
    // tags begin
    mailbox[2] = GET_ARM_MEMORY; // tag identifier
    mailbox[3] = 8; // maximum of request and response value buffer's length.
    mailbox[4] = TAG_REQUEST_CODE;
    mailbox[5] = 0; // value buffer
    mailbox[6] = 0; // value buffer
    // tags end
    mailbox[7] = END_TAG;
    if (mailbox_call(8)) {
        write_uart_string("ARM memory base address: ");
        write_uart_hex(mailbox[5]);
        write_uart_string("ARM memory size in bytes: ");
        write_uart_hex(mailbox[6]);
    } else
        write_uart_string("Failed to get ARM memory information\n");
    
}

