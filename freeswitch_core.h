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

#ifdef _CORE
#define ATTACH_THREADS
#endif

#ifdef WIN32
#define RESULT_FREE(x) CoTaskMemFree(x)
#else
#define RESULT_FREE(x) free(x)
#endif

SWITCH_END_EXTERN_C

#ifdef _CORE
struct switch_core_session {
	char foo[];
};
struct apr_pool_t {
	char foo[];
};
struct switch_channel {
	char foo[];
};
struct apr_thread_t {
	char foo[];
};
struct switch_hash {
	char foo[];
};
struct apr_thread_mutex_t {
	char foo[];
};
struct switch_network_list {
	char foo[];
};
struct switch_xml_binding {
	char foo[];
};
struct apr_sockaddr_t {
	char foo[];
};
struct switch_core_port_allocator {
	char foo[];
};
struct switch_media_bug {
	char foo[];
};
struct switch_rtp {
	char foo[];
};
struct sqlite3_stmt {
	char foo[];
};
struct switch_buffer {
	char foo[];
};
struct switch_ivr_menu {
	char foo[];
};
struct switch_event_node {
	char foo[];
};
struct switch_ivr_digit_stream_parser {
	char foo[];
};
struct sqlite3 {
	char foo[];
};
struct switch_ivr_digit_stream {
	char foo[];
};
struct real_pcre {
	char foo[];
};
struct HashElem {
	char foo[];
};
struct switch_ivr_menu_xml_ctx {
	char foo[];
};
struct apr_file_t {
	char foo[];
};
struct apr_thread_rwlock_t {
	char foo[];
};
struct apr_pollfd_t {
	char foo[];
};
struct apr_queue_t {
	char foo[];
};
struct apr_socket_t {
	char foo[];
};

using namespace System;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

public ref class FreeSwitchCore {
  public:
	static Assembly^ core_dotnet_managed;
	static MethodInfo^ loadMethod;
};

#endif

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
