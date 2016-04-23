#include "mydbg.h"

#define CALL(Context, which, ...)					\
  ((Context)->h && (Context)->h->which					\
   ? (Context)->h->which((Context), ##__VA_ARGS__)			\
   : -1)

void SendDbg(dtls_context_t * ctx, session_t * session, uint8_t * sendbuf, size_t len)
{
    CALL(ctx, write, session, sendbuf, len);
    return;
}
