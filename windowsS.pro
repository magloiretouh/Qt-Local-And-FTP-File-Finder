#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T11:17:15
#
#-------------------------------------------------

include (C:/Qt/Qt5.9.9/5.9.9/Src/qtftp/modules/qt_ftp.pri)

QT       += widgets core gui network ftp

TARGET = windowsS
TEMPLATE = app


SOURCES += main.cpp \
    customcombobox.cpp \
    customtablewidget.cpp \
    extensiondialogadd.cpp \
    filethread.cpp \
    search.cpp \
    settings.cpp \
    windowss.cpp \
    tablekeypresseventlistner.cpp \
    directories.cpp

HEADERS  += \
    customcombobox.h \
    customtablewidget.h \
    extensiondialogadd.h \
    filethread.h \
    search.h \
    settings.h \
    windowss.h \
    tablekeypresseventlistner.h \
    directories.h

FORMS    += windowss.ui \
    extensiondialogadd.ui \
    settings.ui \
    directories.ui

RESOURCES += \
    resource.qrc
