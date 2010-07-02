CONFIG += qtopiaapp
CONFIG -= buildQuicklaunch

HEADERS += DataFusion.h
HEADERS += TestData.h
HEADERS += Sensors.h
HEADERS += SerialComm.h
SOURCES = DataFusion.cpp
SOURCES += TestData.cpp
SOURCES += Sensors.cpp
SOURCES += SerialComm.cpp
SOURCES += main.cpp

INTERFACES += Data_Fusion_Base.ui

TARGET = DataFusion
