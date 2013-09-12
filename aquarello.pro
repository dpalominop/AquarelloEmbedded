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
    barcodescannersettings.h \
    softphone.h \
    widgetcall.h \
    softphonewidget.h \
    serialcommunication.h \
    rs232.h \
    barcodescannersettings.h \
    barcodecatalog.h \
    tcpclient.h \
    display.h \
    linemessages.h \
    udpclient.h \
    udp_client.h \
    audiocodecsettings.h

SOURCES += \
    main.cpp \
    aquarellovideo.cpp \
    picturecatalog.cpp \
    videocatalog.cpp \
    barcodecatalog.cpp \
    aquarellosettings.cpp \
    softphonesettings.cpp \
    contactsettings.cpp \
    barcodescannersettings.cpp \
    softphone.cpp \
    widgetcall.cpp \
    softphonewidget.cpp \
    serialcommunication.cpp \
    rs232.cpp \
    tcpclient.cpp \
    display.cpp \
    linemessages.cpp \
    udpclient.cpp \
    audiocodecsettings.cpp

QT       += core gui multimedia multimediawidgets network webkitwidgets v8
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OTHER_FILES += \
    settings/contacts.xml \
    settings/softphone.xml \
    settings/aquarello.xml \
    settings/scanner.xml

FORMS += \
    widget_call.ui \
    form.ui
