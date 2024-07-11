QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++17
    resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/EndWidget.cpp \
    src/GameController.cpp \
    src/Hold.cpp \
    src/MenuWidget.cpp \
    src/PauseWidget.cpp \
    src/PlayWidget.cpp \
    src/SettingsWidget.cpp \
    src/StartWidget.cpp \
    src/Tap.cpp \
    src/main.cpp \
    src/Note.cpp \
    src/confirmDialog.cpp \
    src/StartWidget.cpp \
    src/ButtonClickSound.h

HEADERS += \
    include/EndWidget.h \
    include/GameController.h \
    include/Hold.h \
    include/MenuWidget.h \
    include/PauseWidget.h \
    include/PlayWidget.h \
    include/SettingsWidget.h \
    include/StartWidget.h \
    include/Tap.h \
    include/Note.h \
    include/confirmDialog.h \
    include/StartWidget.h \
    include/ButtonClickSound.h

INCLUDEPATH += \
    include

FORMS += \
    ui/EndWidget.ui \
    ui/MenuWidget.ui \
    ui/PauseWidget.ui \
    ui/PlayWidget.ui \
    ui/SettingsWidget.ui \
    ui/StartWidget.ui \
    ui/confirmDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
