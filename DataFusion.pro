CONFIG += qtopiaapp
CONFIG -= buildQuicklaunch

HEADERS += DataFusion.h 
HEADERS += TestData.h 
SOURCES = DataFusion.cpp
SOURCES += TestData.cpp
SOURCES += main.cpp

INTERFACES += Data_Fusion_Base.ui

TARGET = DataFusion
