#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libopenblox.la

#######################################
# Build information for each library

# Sources for libopenblox
libopenblox_la_SOURCES = openblox.cpp \
OBException.cpp \
OBLogger.cpp \
ClassFactory.cpp \
TaskScheduler.cpp \
OBEngine.cpp \
lua/OBLua.cpp \
lua/OBLua_OBBase.cpp \
lua/OBLua_OBOS.cpp \
instance/Instance.cpp \
utility.cpp

# Linker options
libopenblox_la_LDFLAGS = $LIRRLICHT_CFLAGS $LCURL_CFLAGS $LLUA_CFLAGS

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libopenblox_la_CPPFLAGS = -I$(top_srcdir)/include $(LLUA_CFLAGS)
