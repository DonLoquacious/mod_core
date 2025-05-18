all: FreeSWITCH.Core.dll

clean:
	rm -fr FreeSWITCH.Core.dll

FreeSWITCH.Core.dll: AssemblyInfo.cs Extensions.cs Loader.cs Log.cs ManagedSession.cs PluginInterfaces.cs PluginManager.cs ScriptPluginManager.cs swig.cs
	dmcs -target:library -out:FreeSWITCH.Core.dll AssemblyInfo.cs Extensions.cs Loader.cs Log.cs ManagedSession.cs PluginInterfaces.cs PluginManager.cs ScriptPluginManager.cs ChannelVariables.cs Util.cs swig.cs XmlSearchBinding.cs

install: FreeSWITCH.Core.dll
	$(INSTALL) FreeSWITCH.Core.dll $(DESTDIR)$(MODINSTDIR)

uninstall:
	$(UNINSTALL) $(MODINSTDIR)/FreeSWITCH.Core.dll

