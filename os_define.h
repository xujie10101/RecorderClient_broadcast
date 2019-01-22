#ifndef OS_DEFINE_HH
#define OS_DEFINE_HH
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include "jw_inline.h"

#define os_htons(x) (((x)<<8) | (((x)>>8)&0xFF))
#define os_htonl(x) (((x)<<24 & 0xFF000000UL)| ((x)<< 8 & 0x00FF0000UL) | \
				     ((x)>> 8 & 0x0000FF00UL)| ((x)>>24 & 0x000000FFUL) )

#define os_ntohl(x)							os_htonl(x)
#define os_ntohs(x)							os_htons(x)

#define os_min(a, b)						(((a) < (b))? (a) : (b))
#define os_max(a, b)						(((a) < (b))? (b) : (a))

#define os_rand								(rand()%(65555-60000+1) + 60000)

#define MAX_LISTEN_CONN						0x400
#define MAX_EVENT_EPOLL                     0x04
#define MAX_NODATA_TIME						0x1E

#define MAX_BUFFER_128						0x80	 //1k
#define MAX_BUFFER_256						0x100	 //1k
#define MAX_BUFFER_512						0x200	 //1k
#define MAX_BUFFER_1KSZ						0x400	 //1k
#define MAX_BUFFER_4KSZ						0x1000	 //4k
#define MAX_BUFFER_8KSZ						0x2000	 //8k
#define MAX_BUFFER_16KSZ					0x4000	 //16k
#define MAX_BUFFER_32KSZ					0x8000	 //32k
#define MAX_BUFFER_64KSZ					0x10000	 //64k
#define MAX_BUFFER_128KSZ					0x20000	 //128k
#define MAX_BUFFER_256KSZ					0x40000	 //256k
#define MAX_BUFFER_512KSZ					0x80000	 //512k
#define MAX_BUFFER_1MSIZE					0x100000 //1M
#define MAX_BUFFER_2MSIZE					0x200000 //2M
#define MAX_BUFFER_4MSIZE					0x400000 //4M
#define RTSP_DATALENGTHIDEND				"\r\n\r\n"

#define SAFE_FREE(P)						if (P){free(P); P = NULL;}
#define SAFE_DELETE(P)                      if (P){delete P; P = NULL;}
#define SAFE_DELETEA(P)                     if (P){delete[] P; P = NULL;}



#ifdef WIN32
#include "win32_base.h"
#pragma warning(disable:4996)
#else
#include "linux_base.h"
#endif

#endif
