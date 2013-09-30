#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
//#include <netinet/in.h>
//#include <unistd.h>

#ifndef _CUFTPD_H
#define _CUFTPD_H

#define CUFTPD_DEBUG(fmt, ...)        cuftpd_debug(__FILE__, __LINE__, fmt, __VA_ARGS__)
#define CUFTPD_ARR_LEN(arr)                (sizeof(arr)/sizeof(arr[0]))

#define CUFTPD_VER                                "1.0"
#define CUFTPD_DEF_SRV_PORT                21
#define CUFTPD_LISTEN_QU_LEN        8
#define CUFTPD_LINE_END                        "\r\n"

#define CUFTPD_OK        0
#define CUFTPD_ERR        (-1)

#define CUFTPD_TIMEOUT  15
#define HOME_DIR "/opt/aquarello/ftpdir"
#define CONFIG_FILE "/opt/aquarello/config.xml"
#define REMOUNT_HOME  0
//#define AQUARELLO_RD_CMD "mount -r -o remount /media/mmcblk0p4"
//#define AQUARELLO_RW_CMD "mount -rw -o remount /media/mmcblk0p4"
 
#define CUFTPD_CHECK_LOGIN()  \
        do { \
                if (!cuftpd_cur_user) { \
                        cuftpd_send_resp(ctrlfd, 530, "first please"); \
                        return CUFTPD_ERR; \
                } \
        } while(0)

struct cuftpd_cmd_struct {
        char *cmd_name;
        int (*cmd_handler)(int ctrlfd, char *cmd_line);
};

struct cuftpd_user_struct {
        char user[128];
        char pass[128];
};


#endif

