TEMPLATE = lib
QT = core gui websockets
TARGET = openblox
win32{
	LIBS += -lws2_32 -lcurldll -lidn
}else{
	LIBS += -lcurl -lboost_system
}
LIBS += -lopenal -lalut -lcrypto -lssl -lz -lOgreMain -llua

CONFIG += dll

HEADERS += raknet/*.h src/ob_lua/*.h src/openblox/*.h src/openblox/instance/*.h src/openblox/type/*.h
SOURCES += raknet/*.cpp src/ob_lua/*.cpp src/openblox/*.cpp src/openblox/instance/*.cpp src/openblox/type/*.cpp
CONFIG += c++11
DEFINES += GXX_EXPERIMENTAL_CXX0X
INCLUDEPATH += src/ob_lua src/openblox src/openblox/instance

freebsd{
	#We need to include headers installed by ports
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib
}
