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
BitStream.cpp \
OBLogger.cpp \
ClassFactory.cpp \
TaskScheduler.cpp \
AssetLocator.cpp \
OBEngine.cpp \
lua/OBLua.cpp \
lua/OBLua_OBBase.cpp \
lua/OBLua_OBOS.cpp \
type/Type.cpp \
type/LuaEnum.cpp \
type/LuaEnumItem.cpp \
type/Enum.cpp \
type/Color3.cpp \
type/VarWrapper.cpp \
type/EventConnection.cpp \
type/Event.cpp \
type/Vector3.cpp \
type/Vector2.cpp \
type/UDim.cpp \
type/UDim2.cpp \
type/CFrame.cpp \
instance/Instance.cpp \
instance/Camera.cpp \
instance/PVInstance.cpp \
instance/Model.cpp \
instance/BindableEvent.cpp \
instance/ServiceProvider.cpp \
instance/Lighting.cpp \
instance/ContentProvider.cpp \
instance/UserInputService.cpp \
instance/BasePlayerGui.cpp \
instance/CoreGui.cpp \
instance/StarterGui.cpp \
instance/RunService.cpp \
instance/TaskScheduler.cpp \
instance/ReplicatedStorage.cpp \
instance/ReplicatedFirst.cpp \
instance/PlayerGui.cpp \
instance/Workspace.cpp \
instance/LogService.cpp \
instance/DataModel.cpp \
instance/Humanoid.cpp \
instance/Folder.cpp \
instance/BasePart.cpp \
instance/MeshPart.cpp \
instance/Part.cpp \
instance/BoolValue.cpp \
instance/DoubleConstrainedValue.cpp \
instance/Color3Value.cpp \
instance/IntConstrainedValue.cpp \
instance/IntValue.cpp \
instance/NumberValue.cpp \
instance/ObjectValue.cpp \
instance/NetworkReplicator.cpp \
instance/ClientReplicator.cpp \
instance/ServerReplicator.cpp \
instance/NetworkPeer.cpp \
instance/NetworkServer.cpp \
instance/NetworkClient.cpp \
instance/GuiBase.cpp \
instance/GuiBase2d.cpp \
instance/GuiObject.cpp \
instance/Frame.cpp \
instance/LayerCollector.cpp \
instance/ScreenGui.cpp \
OBSerializer.cpp \
utility.cpp

# Linker options
libopenblox_la_LDFLAGS = $(LIRRLICHT_LIBS) $(LPUGIXML_LIBS) $(LBULLET_LIBS) $(LCURL_LIBS) $(LENET_LIBS) $(LLUA_LIBS)

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libopenblox_la_CPPFLAGS = -I$(top_srcdir)/include $(LBULLET_CFLAGS) $(LCURL_CFLAGS) $(LENET_CFLAGS) $(LLUA_CFLAGS) -std=c++11 -pthread
