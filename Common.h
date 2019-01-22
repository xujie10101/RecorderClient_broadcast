#ifndef COMMON_HH
#define COMMON_HH


#define BUFSIZE 41000
#define os_free(x) 	if (x){free(x);x = NULL;}
#define os_delete(x) 	if (x){delete(x);x = NULL;}

#pragma pack(4)

typedef struct tagCOMMNET_DEVINFO
{
	unsigned int	nFlag;
	char			szCmd[64];
	unsigned int	dwhNetCommunication;
	unsigned int	dwvsion;
	unsigned int	dwBufSize;
}COMMNET_DEVINFO;

#pragma pack()

#endif // COMMON_H
