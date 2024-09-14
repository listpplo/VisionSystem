<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Main.vi" Type="VI" URL="../Main.vi"/>
		<Item Name="Close.vi" Type="VI" URL="../../viLib/Close.vi"/>
		<Item Name="EnumDevices.vi" Type="VI" URL="../../viLib/EnumDevices.vi"/>
		<Item Name="ExecuteCommandFeature.vi" Type="VI" URL="../../viLib/ExecuteCommandFeature.vi"/>
		<Item Name="GetFrame.vi" Type="VI" URL="../../viLib/GetFrame.vi"/>
		<Item Name="GetIntFeatureValue.vi" Type="VI" URL="../../viLib/GetIntFeatureValue.vi"/>
		<Item Name="GetEnumFeatureValue.vi" Type="VI" URL="../../viLib/GetEnumFeatureValue.vi"/>
		<Item Name="GetEnumFeatureEntrys.vi" Type="VI" URL="../../viLib/GetEnumFeatureEntrys.vi"/>
		<Item Name="GetEnumFeatureSymbol.vi" Type="VI" URL="../../viLib/GetEnumFeatureSymbol.vi"/>
		<Item Name="GetEnumFeatureEntryNum.vi" Type="VI" URL="../../viLib/GetEnumFeatureEntryNum.vi"/>
		<Item Name="IsOpen.vi" Type="VI" URL="../../viLib/IsOpen.vi"/>
		<Item Name="IsGrabbing.vi" Type="VI" URL="../../viLib/IsGrabbing.vi"/>
		<Item Name="OpenDevice.vi" Type="VI" URL="../../viLib/OpenDevice.vi"/>
		<Item Name="PixelConvert.vi" Type="VI" URL="../../viLib/PixelConvert.vi"/>
		<Item Name="ReleaseFrame.vi" Type="VI" URL="../../viLib/ReleaseFrame.vi"/>
		<Item Name="StartGrabbing.vi" Type="VI" URL="../../viLib/StartGrabbing.vi"/>
		<Item Name="StopGrabbing.vi" Type="VI" URL="../../viLib/StopGrabbing.vi"/>
		<Item Name="SaveImageToBmp.vi" Type="VI" URL="../../viLib/SaveImageToBmp.vi"/>
		<Item Name="SetIntFeatureValue.vi" Type="VI" URL="../../viLib/SetIntFeatureValue.vi"/>
		<Item Name="SetEnumFeatureSymbol.vi" Type="VI" URL="../../viLib/SetEnumFeatureSymbol.vi"/>
		<Item Name="SetDoubleFeatureValue.vi" Type="VI" URL="../../viLib/SetDoubleFeatureValue.vi"/>
		<Item Name="GetDoubleFeatureValue.vi" Type="VI" URL="../../viLib/GetDoubleFeatureValue.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="imagedata.ctl" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/imagedata.ctl"/>
				<Item Name="Draw Flattened Pixmap.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Flattened Pixmap.vi"/>
				<Item Name="FixBadRect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/pictutil.llb/FixBadRect.vi"/>
				<Item Name="Calculate Frames per Second.vi" Type="VI" URL="/&lt;vilib&gt;/vision/Calculate Frames per Second.vi"/>
			</Item>
			<Item Name="MVSDKmd.dll" Type="Document" URL="MVSDKmd.dll">
				<Property Name="NI.PreserveRelativePath" Type="Bool">true</Property>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="OneCameraDemo" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{61069199-9F0D-4A3F-80FC-F8AFBE3FD3C4}</Property>
				<Property Name="App_INI_GUID" Type="Str">{A50AC7A5-A724-4BC4-903A-A04D4D1EBB69}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{032B1980-418F-438B-B078-8EA81BC30FFE}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">OneCameraDemo</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../bin</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{639AEBBC-A939-465D-8868-05DBA5883C93}</Property>
				<Property Name="Bld_version.build" Type="Int">13</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">OneCamera.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../bin/NI_AB_PROJECTNAME.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../bin/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{EB9F31A5-9DC6-4DCC-A787-4D6399E083CE}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/Main.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_fileDescription" Type="Str">OneCameraDemo</Property>
				<Property Name="TgtF_internalName" Type="Str">OneCameraDemo</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright ?2023 </Property>
				<Property Name="TgtF_productName" Type="Str">OneCameraDemo</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{8D4085D5-2D0E-4B5A-9183-3ED41E899BC0}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">OneCamera.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
