include $(top_srcdir)/build/modmake.rulesam
MODNAME=mod_core
CORE_DIR=$(switch_srcdir)/src/mod/languages/mod_core/core

mod_LTLIBRARIES = mod_core.la
mod_core_la_SOURCES  = mod_core.cpp freeswitch_core.cpp freeswitch_wrap.cxx
mod_core_la_CFLAGS   = $(AM_CFLAGS)
mod_core_la_CXXFLAGS = $(AM_CXXFLAGS)
mod_core_la_CPPFLAGS = $(SWITCH_AM_CPPFLAGS)
mod_core_la_LIBADD   = $(switch_builddir)/libfreeswitch.la
mod_core_la_LDFLAGS  = -avoid-version -module -no-undefined -shared

CORE_SRCDIR=$(abs_srcdir)/core
BUILT_SOURCES=FreeSWITCH.Core.dll
CS_SRC=$(CORE_SRCDIR)/AssemblyInfo.cs $(CORE_SRCDIR)/Extensions.cs $(CORE_SRCDIR)/Loader.cs $(CORE_SRCDIR)/Log.cs
CS_SRC+=$(CORE_SRCDIR)/ManagedSession.cs $(CORE_SRCDIR)/PluginInterfaces.cs
CS_SRC+=$(CORE_SRCDIR)/PluginManager.cs $(CORE_SRCDIR)/ScriptPluginManager.cs $(CORE_SRCDIR)/ChannelVariables.cs $(CORE_SRCDIR)/Util.cs
CS_SRC+=$(CORE_SRCDIR)/swig.cs $(CORE_SRCDIR)/XmlSearchBinding.cs

freeswitch_core.o: freeswitch_core.h freeswitch_core.cpp

reswig: swigclean freeswitch_wrap.cxx

FreeSWITCH.Core.dll: $(CS_SRC)
	dmcs -target:library -out:FreeSWITCH.Core.dll $(CS_SRC)

install-data-local: FreeSWITCH.Core.dll
	mkdir -p $(DESTDIR)$(modulesdir)/core
	$(INSTALL) FreeSWITCH.Core.dll $(DESTDIR)$(modulesdir)

uninstall:
	rm -fr $(DESTDIR)$(modulesdir)/mod_core.so
	rm -fr $(DESTDIR)$(modulesdir)/FreeSWITCH.Core.dll

#clean:
#	rm -fr FreeSWITCH.Core.dll

swigclean: clean
	rm -f freeswitch_wrap.cxx core/swig.cs

freeswitch_wrap.cxx:
	swig -I../../../include -v -O -c++ -csharp -namespace FreeSWITCH.Native -dllimport mod_core -DSWIG_CSHARP_NO_STRING_HELPER freeswitch.i
	rm -f ./core/swig.cs
	cat *.cs > ./core/swig.cs
	patch -s -p0 -i hack.diff -d ./core
	rm -f *.cs
