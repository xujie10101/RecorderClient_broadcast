#include "jw_util.h"

void os_delay_us(int32_t t_m)
{
#ifdef WIN32
	Sleep(t_m*1000);
#else
	usleep(t_m);
#endif
}

void os_delay_ms(int32_t t_m)
{
#ifdef WIN32
	Sleep(t_m);
#else
	usleep(t_m*1000);
#endif
}


uint32_t os_gettime() 
{
#ifdef WIN32
	return timeGetTime();
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec * 1000ul + tv.tv_usec/1000ul;
#endif
}

char *os_itoa(int32_t i, char *str)
{
	int32_t mod, div = abs(i), index = 0;
	char *start, *end, temp;
	do
	{
		mod = div % 10;
		str[index++] = '0' + mod;
		div = div / 10;
	}while(div != 0);

	if (i < 0)
		str[index++] = '-';

	str[index] = '\0';

	for (start = str, end = str + strlen(str) - 1;
		start < end; start++, end--)
	{
		temp   = *start;
		*start = *end;
		*end   = temp;
	}
	return str;
}

int32_t os_geterr()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}