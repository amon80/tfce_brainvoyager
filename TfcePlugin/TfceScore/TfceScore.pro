#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T09:40:08
#
#-------------------------------------------------

QT       -= core gui

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

TARGET = TfceScore
TEMPLATE = lib

SOURCES += \
    BVQXPluginInterface.cpp \
    nrutil.cpp \
    stat.cpp \
    TfceScore.cpp \
    BinaryString.cpp \
    FindClusters3D.cpp \
    StatisticalMap3D.cpp \
    mystat.cpp

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
    TfceScore.h \
    BinaryString.h \
    FindClusters3D.h \
    StatisticalMap3D.h \
    mystat.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    TfceScore.js

FORMS += \
    TfceScore.ui
