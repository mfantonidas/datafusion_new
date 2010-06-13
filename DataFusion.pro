CONFIG += qtopiaapp
CONFIG -= buildQuicklaunch

HEADERS += DataFusion.h 
HEADERS += TestData.h 
HEADERS += Sensors.h
SOURCES = DataFusion.cpp
SOURCES += TestData.cpp
SOURCES += Sensors.cpp
SOURCES += main.cpp

INTERFACES += Data_Fusion_Base.ui

TARGET = DataFusion
