QT       += core gui multimedia \
    quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimediawidgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    binaryeditor.cpp \
    completer.cpp \
    customedit.cpp \
    filepropertydialog.cpp \
    game2048.cpp \
    highlighter.cpp \
    main.cpp \
    mainwindow.cpp \
    replacedialog.cpp \
    settingsdialog.cpp \
    upperwidget.cpp \
    videoutils.cpp \
    snake.cpp \
    snake_normal.cpp \
    snake_difficult.cpp

HEADERS += \
    binaryeditor.h \
    completer.h \
    customedit.h \
    filepropertydialog.h \
    game2048.h \
    highlighter.h \
    mainwindow.h \
    replacedialog.h \
    settingsdialog.h \
    upperwidget.h \
    videoutils.h \
    snake.h \
    snake_normal.h \
    snake_difficult.h

FORMS += \
    filepropertydialog.ui \
    replacedialog.ui \
    settingsdialog.ui \
    snake_normal.ui \
    snake_difficult.ui \
    snake.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

