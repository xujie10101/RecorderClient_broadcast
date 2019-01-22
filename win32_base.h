#ifndef WINDOWS_DEFINE_HH
#define WINDOWS_DEFINE_HH
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <ws2ipdef.h>
#include <mswsock.h>
#include <process.h>

typedef SOCKET	socket_t;
typedef WSABUF	socket_bufvec_t;


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif



#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"WS2_32.lib")
#pragma comment(lib,"Mswsock.lib")

#pragma warning(disable:4996)
#pragma warning(disable: 6031) 

#define SAFE_RELEASE(P)                     if (P){P->Release(); P = NULL;}
#define SAFE_CLSHANDLE(P)                   if (P){CloseHandle(P); P = NULL;}

#define snprintf							sprintf_s
#endif
