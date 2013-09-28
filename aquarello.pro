TEMPLATE = app
TARGET = aquarello

LIBS += -L/usr/lib/i386-linux-gnu \
    -llinphone \
    -lmediastreamer \
    -lortp

HEADERS += \
    aquarellovideo.h \
    picturecatalog.h \
    videocatalog.h \
    barcodecatalog.h \
    aquarellosettings.h \
    softphonesettings.h \
    contactsettings.h \
    softphone.h \
    widgetcall.h \
    serialcommunication.h \
    barcodecatalog.h \
    tcpclient.h \
    display.h \
    linemessages.h \
    udpclient.h \
    audiocodecsettings.h \
    mybutton.h \
    widgetviewer.h

SOURCES += \
    main.cpp \
    aquarellovideo.cpp \
    picturecatalog.cpp \
    videocatalog.cpp \
    barcodecatalog.cpp \
    aquarellosettings.cpp \
    softphonesettings.cpp \
    contactsettings.cpp \
    softphone.cpp \
    widgetcall.cpp \
    serialcommunication.cpp \
    tcpclient.cpp \
    display.cpp \
    linemessages.cpp \
    udpclient.cpp \
    audiocodecsettings.cpp \
    mybutton.cpp \
    widgetviewer.cpp

QT       += core gui multimedia multimediawidgets network webkitwidgets v8
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

OTHER_FILES += \
    settings/contacts.xml \
    settings/softphone.xml \
    settings/aquarello.xml \
    settings/scanner.xml
