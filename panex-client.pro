######################################################################
# Automatically generated by qmake (2.01a) Wed Mar 13 00:41:18 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . panexapi qslog qt-json res src ui
INCLUDEPATH += . panexapi src qslog qt-json ui
 QT += network
# Input
HEADERS += panexapi/panexapi.h \
           qslog/QsDebugOutput.h \
           qslog/QsLog.h \
           qslog/QsLogDest.h \
           qt-json/json.h \
           src/aboutdialog.h \
           src/configfile.h \
           src/global_include.h \
           src/mainwindow.h \
           src/qpanexapp.h \
           ui/addpatient.h \
           ui/logindialog.h \
           ui/patientview.h \
           ui/settingsdialog.h \
           ui/usersignupdialog.h \
           ui/utils.cpp \
    ui/utils.h \
    ui/changepassword.h \
    ui/uploadappdialog.h \
    form/formpost.h \
    form/formpostinterface.h \
    ui/uploadservicedialog.h \
    ui/patientdataupload.h \
    ui/servicerundialog.h \
    panexapi/patientdataapi.h \
    panexapi/patientapi.h
FORMS += ui/aboutdialog.ui \
         ui/addpatient.ui \
         ui/logindialog.ui \
         ui/mainwindow.ui \
         ui/patientview.ui \
         ui/settings.ui \
         ui/settingsdialog.ui \
         ui/usersignupdialog.ui \
    ui/changepassword.ui \
    ui/uploadappdialog.ui \
    ui/uploadservicedialog.ui \
    ui/patientdataupload.ui \
    ui/servicerundialog.ui
SOURCES += panexapi/panexapi.cpp \
           qslog/QsDebugOutput.cpp \
           qslog/QsLog.cpp \
           qslog/QsLogDest.cpp \
           qt-json/json.cpp \
           src/aboutdialog.cpp \
           src/configfile.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/qpanexapp.cpp \
           ui/addpatient.cpp \
           ui/logindialog.cpp \
           ui/patientview.cpp \
           ui/settingsdialog.cpp \
           ui/usersignupdialog.cpp \
           ui/utils.cpp \
    ui/changepassword.cpp \
    ui/uploadappdialog.cpp \
    form/formpost.cpp \
    ui/uploadservicedialog.cpp \
    ui/patientdataupload.cpp \
    ui/servicerundialog.cpp \
    panexapi/patientdataapi.cpp \
    panexapi/patientapi.cpp
RESOURCES += res/panexres.qrc

OTHER_FILES += \
    LICENSE.txt
