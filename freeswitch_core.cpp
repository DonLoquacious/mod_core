#include <switch.h>
#include <switch_cpp.h>
#include "freeswitch_core.h"

ManagedSession::ManagedSession() : CoreSession() {
}

ManagedSession::ManagedSession(char *uuid) : CoreSession(uuid) {
}

ManagedSession::ManagedSession(switch_core_session_t *session) : CoreSession(session) {
}

bool ManagedSession::begin_allow_threads() {
	return true;
}

bool ManagedSession::end_allow_threads() {
	return true;
}

ManagedSession::~ManagedSession() {
	 ATTACH_THREADS
	 if (session) {
		if (switch_test_flag(this, S_HUP) && !switch_channel_test_flag(channel, CF_TRANSFER)) {
			switch_channel_hangup(channel, SWITCH_CAUSE_NORMAL_CLEARING); setAutoHangup(0);
		}

		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Unsetting CoreSetting for channel.");
		switch_channel_set_private(channel, "CoreSession", NULL);
	}
}

void ManagedSession::check_hangup_hook() {
	ATTACH_THREADS
	if (!hangupDelegate) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "hangupDelegate is null.");
		return;
	}

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Executing hangup delegate.");
	hangupDelegate();
}

switch_status_t ManagedSession::run_dtmf_callback(void *input, switch_input_type_t itype) {
	ATTACH_THREADS
	if (!dtmfDelegate) {
		switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_ERROR, "dtmfDelegate is null.");
		return SWITCH_STATUS_FALSE;
	}

	switch_log_printf(SWITCH_CHANNEL_LOG, SWITCH_LOG_DEBUG, "Executing DTMF delegate.");
	char *result = dtmfDelegate(input, itype);
	switch_status_t status = process_callback_result(result);
	RESULT_FREE(result);

	return status;
}

static switch_status_t stateHangupHandler(switch_core_session_t *session) {
	return SWITCH_STATUS_FALSE;
}
