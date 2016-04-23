#ifndef MYDBG_H
#define MYDBG_H

#include "tinydtls.h"
#include "session.h"
#include "dtls.h"

void SendDbg(dtls_context_t * ctx, session_t * session, uint8_t * sendbuf, size_t len);

#endif
