QT       += core gui
QT += sql
QT += charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addstudentdialog.cpp \
    adminmainwindow.cpp \
    auditlogdialog.cpp \
    backupdialog.cpp \
    changepassworddialog.cpp \
    changequestionsdialog.cpp \
    confirmdeletedialog.cpp \
    databasemanager.cpp \
    editstudentdialog.cpp \
    forgotpassworddialog.cpp \
    gradedialog.cpp \
    logindialog.cpp \
    main.cpp \
    registerdialog.cpp \
    restoredialog.cpp \
    screenshottool.cpp \
    selftest.cpp \
    sqlscript.cpp \
    statsdialog.cpp \
    studentmainwindow.cpp \
    subjectdialog.cpp \
    teachermainwindow.cpp \
    validation.cpp

HEADERS += \
    addstudentdialog.h \
    adminmainwindow.h \
    auditlogdialog.h \
    backupdialog.h \
    changepassworddialog.h \
    changequestionsdialog.h \
    confirmdeletedialog.h \
    databasemanager.h \
    editstudentdialog.h \
    forgotpassworddialog.h \
    gradedialog.h \
    logindialog.h \
    registerdialog.h \
    restoredialog.h \
    screenshottool.h \
    selftest.h \
    sqlscript.h \
    statsdialog.h \
    studentmainwindow.h \
    subjectdialog.h \
    teachermainwindow.h \
    validation.h

FORMS += \
    addstudentdialog.ui \
    adminmainwindow.ui \
    backupdialog.ui \
    changepassworddialog.ui \
    changequestionsdialog.ui \
    confirmdeletedialog.ui \
    editstudentdialog.ui \
    forgotpassworddialog.ui \
    gradedialog.ui \
    logindialog.ui \
    registerdialog.ui \
    restoredialog.ui \
    subjectdialog.ui \
    teachermainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
