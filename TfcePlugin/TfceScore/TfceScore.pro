#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T09:40:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = TfceScore
TEMPLATE = lib

SOURCES += \
    BVQXPluginInterface.cpp \
    nrutil.cpp \
    stat.cpp \
    Tfce.cpp \
    TfceScore.cpp \
    Utilities.cpp 

HEADERS += \
    BVQXPluginInterface.h \
    dlfcn.h \
    global.h \
    nrutil.h \
    Plugin_AR_VMP_Header.h \
    Plugin_DMR_Header.h \
    Plugin_FMR_Header.h \
    Plugin_GLM_Header.h \
    Plugin_MTC_Header.h \
    Plugin_NR_VMP_Header.h \
    Plugin_POI_Header.h \
    Plugin_PRT_Header.h \
    Plugin_SMP_Header.h \
    Plugin_SRF_Header.h \
    Plugin_SSM_Header.h \
    Plugin_VMR_Header.h \
    Plugin_VOI_Header.h \
    Plugin_VTC_Header.h \
    stat.h \
    Tfce.h \
    TfceScore.h \
    Utilities.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=
