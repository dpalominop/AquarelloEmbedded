#ifndef RS232_H
#define RS232_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>


#ifdef __linux__

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#else

#include <windows.h>

#endif

int OpenComport(int, int);
int PollComport(int, unsigned char *, int);
int SendByte(int, unsigned char);
int SendBuf(int, unsigned char *, int);
void CloseComport(int);
void cprintf(int, const char *);
int IsCTSEnabled(int);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
