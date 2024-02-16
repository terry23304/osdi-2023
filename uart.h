#define AUX_ENABLES     ((volatile unsigned int*)(0x3f215004))
#define AUX_MU_IO_REG   ((volatile unsigned int*)(0x3f215040))
#define AUX_MU_IER_REG  ((volatile unsigned int*)(0x3f215044))
#define AUX_MU_IIR_REG  ((volatile unsigned int*)(0x3f215048))
#define AUX_MU_LCR_REG  ((volatile unsigned int*)(0x3f21504C))
#define AUX_MU_MCR_REG  ((volatile unsigned int*)(0x3f215050))
#define AUX_MU_LSR_REG  ((volatile unsigned int*)(0x3f215054))
#define AUX_MU_MSR_REG  ((volatile unsigned int*)(0x3f215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(0x3f21505C))
#define AUX_MU_CNTL_REG ((volatile unsigned int*)(0x3f215060))
#define AUX_MU_STAT_REG ((volatile unsigned int*)(0x3f215064))
#define AUX_MU_BAUD_REG ((volatile unsigned int*)(0x3f215068))

void uart_init();
char read_uart_char();
void write_uart(unsigned int c);
void write_uart_string(char* str);
void write_uart_hex(unsigned int num);