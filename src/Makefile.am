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
PluginManager.cpp \
OBEngine.cpp \
OBRenderUtils.cpp \
FontManager.cpp \
Font.cpp \
OBInputEventReceiver.cpp \
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
type/InputEvent.cpp \
instance/Instance.cpp \
instance/LuaSourceContainer.cpp \
instance/BaseScript.cpp \
instance/Script.cpp \
instance/Camera.cpp \
instance/PVInstance.cpp \
instance/Model.cpp \
instance/BindableEvent.cpp \
instance/ServiceProvider.cpp \
instance/Sky.cpp \
instance/SkyDome.cpp \
instance/SkyBox.cpp \
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
instance/Player.cpp \
instance/Players.cpp \
instance/LogService.cpp \
instance/DataModel.cpp \
instance/HttpService.cpp \
instance/Humanoid.cpp \
instance/Folder.cpp \
instance/BasePart.cpp \
instance/MeshPart.cpp \
instance/Part.cpp \
instance/BaseValue.cpp \
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
instance/RemoteEvent.cpp \
instance/GuiBase.cpp \
instance/GuiBase2d.cpp \
instance/GuiObject.cpp \
instance/Frame.cpp \
instance/LayerCollector.cpp \
instance/ScreenGui.cpp \
instance/GuiLabel.cpp \
instance/ImageLabel.cpp \
OBSerializer.cpp \
utility.cpp

# Linker options
libopenblox_la_LDFLAGS = $(LIRRLICHT_LIBS) $(LSDL2_LIBS) $(LSDL2_IMAGE_LIBS) $(LFREETYPE_LIBS) $(LFONTCONFIG_LIBS) $(LPUGIXML_LIBS) $(LBULLET_LIBS) $(LCURL_LIBS) $(LENET_LIBS) $(LLUA_LIBS)

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libopenblox_la_CPPFLAGS = -I$(top_srcdir)/include $(LSDL2_CFLAGS) $(LFREETYPE_CFLAGS) $(LFONTCONFIG_CFLAGS) $(LBULLET_CFLAGS) $(LCURL_CFLAGS) $(LENET_CFLAGS) $(LLUA_CFLAGS) -std=c++11 -pthread
