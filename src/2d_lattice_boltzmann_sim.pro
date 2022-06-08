QT += core
# QT += core5compat
QT += gui
QT += xml
QT += xmlpatterns
# QT += widgets


TARGET = 2d_lattice_boltzmann_sim
CONFIG   += console
CONFIG   -= app_bundle
CONFIG -= qt

TEMPLATE = app
DEPENDPATH += .

INCLUDEPATH += /include/QtOpenGL /include/Qt /include/QtGui 
#/usr/include/QtOpenGL /usr/include/qt5 /usr/include/qt5/QtGui 
#/usr/include/boost
# /usr/include
# /usr/include/c++/9
QMAKE_CXXFLAGS_RELEASE    = -O4 -march=native -ffast-math -funroll-loops
#QMAKE_CXXFLAGS_RELEASE +=              -fopenmp
# -Wall \
#-Werror \

CONFIG += release \
          warn_on \
          qt \
#          opengl \
          thread

#LIBS +=  -lm -lgsl  -lgslcblas
# -fopenmp
DESTDIR= ../bin
OBJECTS_DIR= ../obj

HEADERS += \
./stringstuff/stringstuffbasic.h \
./math/mathstuffbasic.h \
./math/lvector.h \
./io/inpout.h \
./latticeboltzmann/latticeboltzmann.h \
./latticeboltzmann/xmlio.h



SOURCES += main.cpp \
./math/mathstuffbasic.cpp \
./math/lvector.cpp \
./stringstuff/stringstuffbasic.cpp \
./io/inpout.cpp \
./latticeboltzmann/latticeboltzmann.cpp \
./latticeboltzmann/xmlio.cpp



#

RESOURCES +=

OTHER_FILES += 
