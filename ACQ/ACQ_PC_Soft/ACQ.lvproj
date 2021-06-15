<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="20008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="1_OnDemand.vi" Type="VI" URL="../1_OnDemand.vi"/>
		<Item Name="2_OnInterrupts.vi" Type="VI" URL="../2_OnInterrupts.vi"/>
		<Item Name="2_OnInterrupts_Raw_Slow.vi" Type="VI" URL="../2_OnInterrupts_Raw_Slow.vi"/>
		<Item Name="3_IRQ_50ksps.vi" Type="VI" URL="../3_IRQ_50ksps.vi"/>
		<Item Name="4_DMA_1Msps.vi" Type="VI" URL="../4_DMA_1Msps.vi"/>
		<Item Name="4_DMA_Mem2Mem.vi" Type="VI" URL="../4_DMA_Mem2Mem.vi"/>
		<Item Name="Prezentacja_OnDemand_.vi" Type="VI" URL="../Prezentacja_OnDemand_.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Dynamic To Waveform Array.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Dynamic To Waveform Array.vi"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="ex_Modify Signal Name.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_Modify Signal Name.vi"/>
				<Item Name="ex_Modify Signals Names.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_Modify Signals Names.vi"/>
				<Item Name="exnSpectralBlock clear array magnitudes.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock clear array magnitudes.vi"/>
				<Item Name="exnSpectralBlock clear array phases.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock clear array phases.vi"/>
				<Item Name="exnSpectralBlock clear DDT y array.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock clear DDT y array.vi"/>
				<Item Name="exnSpectralBlock clear magnitude.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock clear magnitude.vi"/>
				<Item Name="exnSpectralBlock clear phase.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock clear phase.vi"/>
				<Item Name="exnSpectralBlock mod exp dyn x attribs.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock mod exp dyn x attribs.vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS array WFM (CDB).vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS array WFM (CDB).vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS array WFM.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS array WFM.vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS DDT.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS DDT.vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS poly.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS poly.vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS WFM (CDB).vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS WFM (CDB).vi"/>
				<Item Name="exnSpectralBlock spectral measurements RMS WFM.vi" Type="VI" URL="/&lt;vilib&gt;/express/express analysis/Spectral/Block/exnSpectralBlock spectral measurements RMS WFM.vi"/>
				<Item Name="NI_AALBase.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/NI_AALBase.lvlib"/>
				<Item Name="NI_AALPro.lvlib" Type="Library" URL="/&lt;vilib&gt;/Analysis/NI_AALPro.lvlib"/>
				<Item Name="NI_MABase.lvlib" Type="Library" URL="/&lt;vilib&gt;/measure/NI_MABase.lvlib"/>
				<Item Name="NI_MAPro.lvlib" Type="Library" URL="/&lt;vilib&gt;/measure/NI_MAPro.lvlib"/>
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
				<Item Name="Waveform Array To Dynamic.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/transition.llb/Waveform Array To Dynamic.vi"/>
			</Item>
			<Item Name="lvanlys.dll" Type="Document" URL="/&lt;resource&gt;/lvanlys.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="3_IRQ_50ksps" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{221848DB-F18D-4DB1-B128-EA483E820399}</Property>
				<Property Name="App_INI_GUID" Type="Str">{D13F6242-B142-4FD2-970F-0F60594DD5C6}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="App_serverType" Type="Int">1</Property>
				<Property Name="App_winsec.description" Type="Str">http://www.AGH.com</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{828BF0CE-EA4A-4636-98C5-378E975CE010}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">3_IRQ_50ksps</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../Builts</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{10DC20FC-8857-47F9-865B-267E16043B9C}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">3_IRQ_50ksps.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../Builts/3_IRQ_50ksps.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../Builts/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{3C2212D2-A6B8-4CE0-9E11-8D4948FB0F0A}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/3_IRQ_50ksps.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">AGH</Property>
				<Property Name="TgtF_fileDescription" Type="Str">3_IRQ_50ksps</Property>
				<Property Name="TgtF_internalName" Type="Str">3_IRQ_50ksps</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2017 AGH</Property>
				<Property Name="TgtF_productName" Type="Str">3_IRQ_50ksps</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{134A8F93-26CA-4979-AFDB-3A5B9CEDA519}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">3_IRQ_50ksps.exe</Property>
			</Item>
			<Item Name="4_DMA_1Msps" Type="EXE">
				<Property Name="App_copyErrors" Type="Bool">true</Property>
				<Property Name="App_INI_aliasGUID" Type="Str">{82CD3588-5613-436A-B98F-034FBECE01CF}</Property>
				<Property Name="App_INI_GUID" Type="Str">{2E49B553-6699-4A30-95A8-1F0525294601}</Property>
				<Property Name="App_serverConfig.httpPort" Type="Int">8002</Property>
				<Property Name="App_serverType" Type="Int">1</Property>
				<Property Name="App_winsec.description" Type="Str">http://www.AGH.com</Property>
				<Property Name="Bld_autoIncrement" Type="Bool">true</Property>
				<Property Name="Bld_buildCacheID" Type="Str">{918FDDD2-6C20-4A66-BCAB-1911BAD1B036}</Property>
				<Property Name="Bld_buildSpecName" Type="Str">4_DMA_1Msps</Property>
				<Property Name="Bld_excludeInlineSubVIs" Type="Bool">true</Property>
				<Property Name="Bld_excludeLibraryItems" Type="Bool">true</Property>
				<Property Name="Bld_excludePolymorphicVIs" Type="Bool">true</Property>
				<Property Name="Bld_localDestDir" Type="Path">../Builts</Property>
				<Property Name="Bld_localDestDirType" Type="Str">relativeToProject</Property>
				<Property Name="Bld_modifyLibraryFile" Type="Bool">true</Property>
				<Property Name="Bld_previewCacheID" Type="Str">{E0F72541-429E-47D7-A67D-BB13FF14419F}</Property>
				<Property Name="Bld_version.build" Type="Int">1</Property>
				<Property Name="Bld_version.major" Type="Int">1</Property>
				<Property Name="Destination[0].destName" Type="Str">4_DMA_1Msps.exe</Property>
				<Property Name="Destination[0].path" Type="Path">../Builts/4_DMA_1Msps.exe</Property>
				<Property Name="Destination[0].path.type" Type="Str">relativeToProject</Property>
				<Property Name="Destination[0].preserveHierarchy" Type="Bool">true</Property>
				<Property Name="Destination[0].type" Type="Str">App</Property>
				<Property Name="Destination[1].destName" Type="Str">Support Directory</Property>
				<Property Name="Destination[1].path" Type="Path">../Builts/data</Property>
				<Property Name="Destination[1].path.type" Type="Str">relativeToProject</Property>
				<Property Name="DestinationCount" Type="Int">2</Property>
				<Property Name="Source[0].itemID" Type="Str">{3C2212D2-A6B8-4CE0-9E11-8D4948FB0F0A}</Property>
				<Property Name="Source[0].type" Type="Str">Container</Property>
				<Property Name="Source[1].destinationIndex" Type="Int">0</Property>
				<Property Name="Source[1].itemID" Type="Ref">/My Computer/4_DMA_1Msps.vi</Property>
				<Property Name="Source[1].sourceInclusion" Type="Str">TopLevel</Property>
				<Property Name="Source[1].type" Type="Str">VI</Property>
				<Property Name="SourceCount" Type="Int">2</Property>
				<Property Name="TgtF_companyName" Type="Str">AGH</Property>
				<Property Name="TgtF_fileDescription" Type="Str">4_DMA_1Msps</Property>
				<Property Name="TgtF_internalName" Type="Str">4_DMA_1Msps</Property>
				<Property Name="TgtF_legalCopyright" Type="Str">Copyright © 2017 AGH</Property>
				<Property Name="TgtF_productName" Type="Str">4_DMA_1Msps</Property>
				<Property Name="TgtF_targetfileGUID" Type="Str">{B4EB6ACE-5EE3-4930-9248-CA44B32BAE00}</Property>
				<Property Name="TgtF_targetfileName" Type="Str">4_DMA_1Msps.exe</Property>
			</Item>
		</Item>
	</Item>
</Project>
