QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -g -W -Wall -Werror -Wfloat-equal -Wunused -Wnon-virtual-dtor -isystem $$[QT_INSTALL_HEADERS]

OBJECTS_DIR = build/obj
MOC_DIR     = build/moc
DESTDIR     = build/bin

MAIN_H = CommonDefs.h MainWindow.h
MAIN_S = main.cpp MainWindow.cpp

DRAW_H = DrawArea.h Approximation.h R2Geometry.h
DRAW_S = DrawArea.cpp Approximation.cpp R2Geometry.cpp

INFO_H = InfoDisplay.h Data.h 
INFO_S = InfoDisplay.cpp Data.cpp

EXTRA_DEBUG = trapfpe_gcc.c

HEADERS += \
		$$MAIN_H \
		$$INFO_H \
		$$DRAW_H 

SOURCES += \
		$$MAIN_S \
		$$INFO_S \
		$$DRAW_S \
		$$EXTRA_DEBUG


