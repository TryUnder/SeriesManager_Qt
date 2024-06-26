QT       += sql core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    LoginAccountDialog.cpp \
    accountmanager.cpp \
    createaccountDialog.cpp \
    databasemanager.cpp \
    deleteaccount.cpp \
    main.cpp \
    mainwindow.cpp \
    myanimelistdialog.cpp \
    remindpassword.cpp \
    series.cpp \
    watchmanager.cpp

HEADERS += \
    LoginAccountDialog.h \
    accountmanager.h \
    createaccountDialog.h \
    databasemanager.h \
    deleteaccount.h \
    mainwindow.h \
    myanimelistdialog.h \
    remindpassword.h \
    series.h \
    watchmanager.h

FORMS += \
    createAccount.ui \
    deleteaccount.ui \
    loginAccount.ui \
    mainwindow.ui \
    myanimelistdialog.ui \
    remindpassword.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
