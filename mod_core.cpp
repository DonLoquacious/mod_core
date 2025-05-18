#include <switch.h>
#include "freeswitch_core.h"

#ifdef _CORE
#include <mscoree.h>

using namespace System;
using namespace System::Runtime::InteropServices;
#define MOD_CORE_VERSION "CoreCLR Version"
#endif

SWITCH_BEGIN_EXTERN_C

SWITCH_MODULE_LOAD_FUNCTION(mod_core_load);
SWITCH_MODULE_DEFINITION_EX(mod_core, mod_core_load, NULL, NULL, SMODF_GLOBAL_SYMBOLS);

SWITCH_STANDARD_API(core_run_api_function);		/* ExecuteBackground */
SWITCH_STANDARD_API(core_api_function);			/* Execute */
SWITCH_STANDARD_APP(core_app_function);			/* Run */
SWITCH_STANDARD_API(core_reload_api_function);	/* Reload */
SWITCH_STANDARD_API(core_list_api_function);	/* List modules */

#define MOD_CORE_ASM_NAME "FreeSwitch.Core"
#define MOD_CORE_ASM_V1 1
#define MOD_CORE_ASM_V2 0
#define MOD_CORE_ASM_V3 0
#define MOD_CORE_ASM_V4 0
#define MOD_CORE_DLL MOD_CORE_ASM_NAME ".dll"
#define MOD_CORE_IMAGE_NAME "FreeSwitch"
#define MOD_CORE_CLASS_NAME "Loader"

mod_core_globals core_globals = { 0 };

typedef int (*runFunction)(const char *data, void *sessionPtr);
typedef int (*executeFunction)(const char *cmd, void *stream, void *Event);
typedef int (*executeBackgroundFunction)(const char* cmd);
typedef int (*reloadFunction)(const char* cmd);
typedef int (*listFunction)(const char *cmd, void *stream, void *Event);
static runFunction runDelegate;
static executeFunction executeDelegate;
static executeBackgroundFunction executeBackgroundDelegate;
static reloadFunction reloadDelegate;
static listFunction listDelegate;

SWITCH_MOD_DECLARE_NONSTD(void) InitManagedDelegates(runFunction run, executeFunction execute, executeBackgroundFunction executeBackground, reloadFunction reload, listFunction list)
{
    runDelegate = run;
    executeDelegate = execute;
    executeBackgroundDelegate = executeBackground;
    reloadDelegate = reload;
    listDelegate = list;
}

SWITCH_MOD_DECLARE_NONSTD(void) InitManagedSession(ManagedSession *session, inputFunction dtmfDelegate, hangupFunction hangupDelegate)
{
    switch_assert(session);
    if (!session) {
        return;
    }
    session->setDTMFCallback(NULL, (char *)"");
    session->setHangupHook(NULL);
    session->dtmfDelegate = dtmfDelegate;
    session->hangupDelegate = hangupDelegate;
}

#ifdef _CORE
switch_status_t loadRuntime()
{
    char filename[256];
    switch_snprintf(filename, 256, "%s%s%s", SWITCH_GLOBAL_dirs.mod_dir, SWITCH_PATH_SEPARATOR, MOD_CORE_DLL);

    wchar_t modpath[256];
    mbstowcs(modpath, filename, 255);
    try {
        FreeSwitchCore::core_dotnet_managed = Assembly::LoadFrom(gcnew String(modpath));
    }
    catch(Exception^ ex) {
        IntPtr msg = Marshal::StringToHGlobalAnsi(ex->ToString());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Assembly::LoadFrom failed: %s\n", static_cast<const char*>(msg.ToPointer()));
        Marshal::FreeHGlobal(msg);
        return SWITCH_STATUS_FALSE;
    }

    return SWITCH_STATUS_SUCCESS;
}

switch_status_t findLoader()
{
    try {
        FreeSwitchCore::loadMethod = FreeSwitchCore::core_dotnet_managed->GetType(MOD_CORE_IMAGE_NAME "." MOD_CORE_CLASS_NAME)->GetMethod("Load");
    }
    catch(Exception^ ex) {
        IntPtr msg = Marshal::StringToHGlobalAnsi(ex->ToString());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Could not load " MOD_CORE_IMAGE_NAME "." MOD_CORE_CLASS_NAME " class: %s\n", static_cast<const char*>(msg.ToPointer()));
        Marshal::FreeHGlobal(msg);

        return SWITCH_STATUS_FALSE;
    }
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Found all %s.%s functions.\n", MOD_CORE_IMAGE_NAME, MOD_CORE_CLASS_NAME);

    return SWITCH_STATUS_SUCCESS;
}
#endif

SWITCH_MODULE_LOAD_FUNCTION(mod_core_load)
{
    int success;

    *module_interface = switch_loadable_module_create_module_interface(pool, modname);
    switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_INFO, "Loading mod_core (Common Language Infrastructure), " MOD_CORE_VERSION "\n");

    core_globals.pool = pool;

    if (loadRuntime() != SWITCH_STATUS_SUCCESS) {
        return SWITCH_STATUS_FALSE;
    }

    if (findLoader() != SWITCH_STATUS_SUCCESS) {
        return SWITCH_STATUS_FALSE;
    }

#ifdef _CORE
	try {
        Object^ objResult = FreeSwitchCore::loadMethod->Invoke(nullptr, nullptr);
        success = *reinterpret_cast<bool^>(objResult);
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Load completed successfully.\n");
    }
    catch(Exception^ ex) {
        IntPtr msg = Marshal::StringToHGlobalAnsi(ex->ToString());
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Load did not return true. %s\n", static_cast<const char*>(msg.ToPointer()));
        Marshal::FreeHGlobal(msg);

        return SWITCH_STATUS_FALSE;
    }
#endif

    if (success) {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Load completed successfully.\n");
    } else {
        switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Load did not return true.\n");

        return SWITCH_STATUS_FALSE;
    }

    switch_api_interface_t *api_interface;
    switch_application_interface_t *app_interface;

    SWITCH_ADD_API(api_interface, "corerun", "Run a module (ExecuteBackground)", core_run_api_function, "<module> [<args>]");
    SWITCH_ADD_API(api_interface, "core", "Run a module as an API function (Execute)", core_api_function, "<module> [<args>]");
    SWITCH_ADD_APP(app_interface, "core", "Run CLI App", "Run an App on a channel", core_app_function, "<modulename> [<args>]", SAF_SUPPORT_NOMEDIA);
    SWITCH_ADD_API(api_interface, "corereload", "Force [re]load of a file", core_reload_api_function, "<filename>");
    SWITCH_ADD_API(api_interface, "corelist", "Log the list of available APIs and Apps", core_list_api_function, "");

    return SWITCH_STATUS_NOUNLOAD;
}

#ifdef _CORE
#pragma unmanaged
#endif

SWITCH_STANDARD_API(core_run_api_function)
{
	if (zstr(cmd)) {
		stream->write_function(stream, "-ERR no args specified!\n");

		return SWITCH_STATUS_SUCCESS;
	}

	if (executeBackgroundDelegate(cmd)) {
		stream->write_function(stream, "+OK\n");
	} else {
		stream->write_function(stream, "-ERR ExecuteBackground returned false (unknown module or exception?).\n");
	}

	return SWITCH_STATUS_SUCCESS;
}

SWITCH_STANDARD_API(core_api_function)
{
	if (zstr(cmd)) {
		stream->write_function(stream, "-ERR no args specified!\n");

		return SWITCH_STATUS_SUCCESS;
	}

	if (!(executeDelegate(cmd, stream, stream->param_event))) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Execute failed for %s (unknown module or exception).\n", cmd);
	}

	return SWITCH_STATUS_SUCCESS;
}

SWITCH_STANDARD_APP(core_app_function)
{
	if (zstr(data)) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "No args specified!\n");

		return;
	}

	if (!(runDelegate(data, session))) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Application run failed for %s (unknown module or exception).\n", data);
	}
}

SWITCH_STANDARD_API(core_reload_api_function)
{
	if (zstr(cmd)) {
		stream->write_function(stream, "-ERR no args specified!\n");

		return SWITCH_STATUS_SUCCESS;
	}

	if (!(reloadDelegate(cmd))) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "Execute failed for %s (unknown module or exception).\n", cmd);
	}

	return SWITCH_STATUS_SUCCESS;
}

SWITCH_STANDARD_API(core_list_api_function)
{
	listDelegate(cmd, stream, stream->param_event);

	return SWITCH_STATUS_SUCCESS;
}

SWITCH_END_EXTERN_C
