#ifndef GLOBALFUNCTION_HH
#define GLOBALFUNCTION_HH
#include "os_define.h"

char *os_itoa(int32_t i, char *str);

void os_delay_us(int32_t t_m);

void os_delay_ms(int32_t t_m);

uint32_t os_gettime();

int32_t os_geterr();

#endif