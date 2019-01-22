#ifndef LINUX_DEFINE_HH
#define LINUX_DEFINE_HH

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <errno.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

typedef int socket_t;
typedef struct iovec socket_bufvec_t;


#define SOCKET_ERROR			-1
#define INVALID_SOCKET			-1
#define INFINITE				0xFFFFFFFF

#endif

