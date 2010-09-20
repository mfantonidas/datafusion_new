CONFIG += qtopiaapp
CONFIG -= buildQuicklaunch
CONFIG+= thread

HEADERS += DataFusion.h
HEADERS += TestData.h
HEADERS += Sensors.h
;;HEADERS += SerialComm.h
HEADERS += Serial.h
SOURCES = DataFusion.cpp
SOURCES += TestData.cpp
SOURCES += Sensors.cpp
;;SOURCES += SerialComm.cpp
SOURCES += serial_test1.cpp
SOURCES += main.cpp

INTERFACES += Data_Fusion_Base.ui

TARGET = DataFusion
