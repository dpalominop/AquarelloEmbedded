QT += network \
    webkitwidgets \
    widgets \
    multimedia \
    ftp

# Input
HEADERS += barcode.h base.h \
    configuration.h \
    welcomeconfiguration.h \
    tcpclient.h \
    display.h \
    udpclient.h \
    linemessages.h \
    tcpserver.h \
    cuftpd.h \
    ftpclient.h \
    configftp.h

SOURCES += base.cpp main.cpp \
    configuration.cpp \
    welcomeconfiguration.cpp \
    tcpclient.cpp \
    display.cpp \
    udpclient.cpp \
    linemessages.cpp \
    tcpserver.cpp \
    ftpclient.cpp \
    configftp.cpp

RESOURCES +=
