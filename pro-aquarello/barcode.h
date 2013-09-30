#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
int open_ttyPort(char *,int);
int init_Port(int,int );
void close_ttyPort(int );
static int fd;
struct termios T_old;
static int fdopenf;
void barcodeSlot();
