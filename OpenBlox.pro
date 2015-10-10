#TODO
QT = core gui websockets
TARGET = openblox
win32{
	LIBS += -lws2_32 -lcurldll -lidn
}else{
	LIBS += -lGL -lGLU -lcurl
}
LIBS += -lopenal -lalut -lcrypto -lssl -lz -lOgreMain -llua

CONFIG += staticlib

HEADERS += raknet/*.h src/ob_lua/*.h src/openblox/instance/*.h
SOURCES += raknet/*.cpp src/ob_lua/*.cpp src/openblox/*.cpp src/openblox/instance/*.cpp
CONFIG += c++11
DEFINES += GXX_EXPERIMENTAL_CXX0X
INCLUDEPATH += src/ob_lua src/openblox src/openblox/instance
