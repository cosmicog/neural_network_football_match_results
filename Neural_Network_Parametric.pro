#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T03:19:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Neural_Network_Parametric
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog1.cpp

HEADERS  += mainwindow.h \
    neuralnetwork.h \
    dialog1.h

FORMS    += mainwindow.ui \
    dialog1.ui

OTHER_FILES += thinking.png \
               data.txt \
               tags.txt \
               perfPlot.m \
               perfPlotAlpha.m

# MATLAB
INCLUDEPATH += /ogh/MATLAB/R2013b/extern/include
QMAKE_CFLAGS += -O3 -I /ogh/MATLAB/R2013b/extern/include -ansi -D_GNU_SOURCE -f /usr/local/MATLAB/R2013a/bin/engopts.sh
LIBS += -L"/ogh/MATLAB/R2013b/bin/glnxa64" -leng -lmat -lmex -lmx -Wl,-rpath=/ogh/MATLAB/R2013b/bin/glnxa64 -lfreetype
