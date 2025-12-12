QT       += core gui widgets

TARGET = LibraryManagementSystem
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/BookList.cpp

HEADERS += \
    src/MainWindow.h \
    src/BookList.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
