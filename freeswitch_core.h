#ifndef FREESWITCH_CORE_H
#define FREESWITCH_CORE_H

SWITCH_BEGIN_EXTERN_C
#include <switch.h>
#include <switch_cpp.h>
typedef void (*hangupFunction) (void);
typedef char *(*inputFunction) (void *, switch_input_type_t);

#ifndef SWIG
struct mod_core_globals {
	switch_memory_pool_t *pool;
};
typedef struct mod_core_globals mod_core_globals;
extern mod_core_globals core_globals;
#endif

#define RESULT_FREE(x) free(x)

SWITCH_END_EXTERN_C

#ifdef _CORE

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

public ref class FreeSwitchCore {
  public:
	static Assembly^ core_dotnet_managed;
	static MethodInfo^ loadMethod;
};

class ManagedSession:public CoreSession {
  public:
	ManagedSession(void);
	ManagedSession(char *uuid);
	ManagedSession(switch_core_session_t *session);
	virtual ~ ManagedSession();

	virtual bool begin_allow_threads();
	virtual bool end_allow_threads();
	virtual void check_hangup_hook();
	virtual switch_status_t run_dtmf_callback(void *input, switch_input_type_t itype);

	inputFunction dtmfDelegate;
	hangupFunction hangupDelegate;
};

#endif
#endif
