QT       += core gui
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    V1/SterowanieKonsola/ArduinoCommunicator.cpp \
    V1/SterowanieKonsola/Calibration.cpp \
    V1/SterowanieKonsola/CommandCreator.cpp \
    V1/SterowanieKonsola/Commander.cpp \
    V1/SterowanieKonsola/Messenger.cpp \
#    V1/SterowanieKonsola/SterowanieKonsola.cpp \
#    V1/SterowanieKonsola/Test.cpp \
    V1/SterowanieKonsola/MicroscopeCamera.cpp \
    V1/SterowanieKonsola/rs232.c \
    controller.cpp \
    main.cpp \
    model.cpp

HEADERS += \
    V1/SterowanieKonsola/ArduinoCommunicator.h \
    V1/SterowanieKonsola/CommandCreator.h \
    V1/SterowanieKonsola/Commander.h \
    V1/SterowanieKonsola/Messenger.h \
#    V1/SterowanieKonsola/SterowanieKonsola.aps \
#    V1/SterowanieKonsola/SterowanieKonsola.rc \
#    V1/SterowanieKonsola/Test.h \
    V1/SterowanieKonsola/MicroscopeCamera.h \
    V1/SterowanieKonsola/resource.h \
    V1/SterowanieKonsola/rs232.h \
    V1/SterowanieKonsola/timer.h \
    controller.h \
    model.h

FORMS += \
    controller_form.ui

CONFIG += lrelease

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .vs/ProjectSettings.json \
    .vs/VSWorkspaceState.json \
    .vs/slnx.sqlite \
    .vscode/launch.json \
    V1/.gitkeep \
    V1/.vs/SterowanieKonsola/v16/.suo \
    V1/.vs/SterowanieKonsola/v16/Browse.VC.db \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/397d54028dacdac9/CALIBRATION.ipch \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/72b504b1f19253eb/COMMANDER.ipch \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/a05927a943cab0fa/COMMANDCREATOR.ipch \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/ae0b8fabf1e3eaa2/MESSENGER.ipch \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/c130d90ef0f290bd/STEROWANIEKONSOLA.ipch \
    V1/.vs/SterowanieKonsola/v16/ipch/AutoPCH/ce53137ce153a60a/ARDUINOCOMMUNICATOR.ipch \
    V1/Debug/SterowanieKonsola.exe \
    V1/Debug/SterowanieKonsola.pdb \
    V1/SterowanieKonsola.sln \
    V1/SterowanieKonsola/.vs/ProjectSettings.json \
    V1/SterowanieKonsola/.vs/SterowanieKonsola/v16/.suo \
    V1/SterowanieKonsola/.vs/SterowanieKonsola/v16/Browse.VC.db \
    V1/SterowanieKonsola/.vs/VSWorkspaceState.json \
    V1/SterowanieKonsola/.vs/slnx.sqlite \
    V1/SterowanieKonsola/.vscode/c_cpp_properties.json \
    V1/SterowanieKonsola/.vscode/settings.json \
    V1/SterowanieKonsola/ArduinoMain.txt \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/CL.command.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/CL.read.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/CL.write.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/SterowanieKonsola.lastbuildstate \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/link.command.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/link.read.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/link.write.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/rc.command.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/rc.read.1.tlog \
    V1/SterowanieKonsola/Debug/Sterowan.a41cd588.tlog/rc.write.1.tlog \
    V1/SterowanieKonsola/Debug/SterowanieKonsola.exe.recipe \
    V1/SterowanieKonsola/Debug/SterowanieKonsola.ilk \
    V1/SterowanieKonsola/Debug/SterowanieKonsola.log \
    V1/SterowanieKonsola/Debug/SterowanieKonsola.res \
    V1/SterowanieKonsola/Debug/vc142.idb \
    V1/SterowanieKonsola/Debug/vc142.pdb \
    V1/SterowanieKonsola/OdbieranieSily.txt \
    V1/SterowanieKonsola/StaryMain.txt \
    V1/SterowanieKonsola/SterowanieKonsola.vcxproj \
    V1/SterowanieKonsola/SterowanieKonsola.vcxproj.filters \
    V1/SterowanieKonsola/TestowanieOdpArd.txt \
    V1/SterowanieKonsola/x64/Debug/Sterowan.a41cd588.tlog/CL.command.1.tlog \
    V1/SterowanieKonsola/x64/Debug/Sterowan.a41cd588.tlog/CL.read.1.tlog \
    V1/SterowanieKonsola/x64/Debug/Sterowan.a41cd588.tlog/CL.write.1.tlog \
    V1/SterowanieKonsola/x64/Debug/Sterowan.a41cd588.tlog/SterowanieKonsola.lastbuildstate \
    V1/SterowanieKonsola/x64/Debug/Sterowan.a41cd588.tlog/unsuccessfulbuild \
    V1/SterowanieKonsola/x64/Debug/SterowanieKonsola.log \
    V1/SterowanieKonsola/x64/Debug/vc142.idb \
    V1/SterowanieKonsola/x64/Debug/vc142.pdb

win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world453
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world453d
else:unix: LIBS += -LD:/OpenCV/opencv/build/x64/vc15/lib/ -lopencv_world453

INCLUDEPATH += D:/OpenCV/opencv/build/include
DEPENDPATH += D:/OpenCV/opencv/build/include
