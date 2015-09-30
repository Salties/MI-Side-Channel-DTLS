#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>

#include "tinydtls.h"

#ifndef DEBUG
#define DEBUG DEBUG_PRINT
#endif
#include "net/ip/uip-debug.h"
#include "net/ip/udp-socket.h"

#include "debug.h"
#include "dtls.h"

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[UIP_LLIPH_LEN])

#define MAX_PAYLOAD_LEN 120

//static struct uip_udp_conn *server_conn;
static struct udp_socket server_conn;
void DtlsServerCB (struct udp_socket *c, void *ptr,
		   const uip_ipaddr_t * source_addr, uint16_t source_port,
		   const uip_ipaddr_t * dest_addr, uint16_t dest_port,
		   const uint8_t * data, uint16_t datalen);
static dtls_context_t *dtls_context = NULL;

static char *INVALID_COMMAND = "INVALID COMMAND\r\n";

#define SPACE 20
int
ReadSensors (uint8 * data, size_t * plen)
{
  int count, value, size;

  for (count = 0, value = 0; count < SPACE; count++)
    {
      value += random_rand () % 100;
    }
  value /= SPACE;
  size = snprintf ((char *) data, *plen, "READ: %d\r\n", value);
  *plen = size;

  return size;
}

#define MAX_BUF 128
int
read_from_peer (struct dtls_context_t *ctx,
		session_t * session, uint8 * data, size_t len)
{
  uint8 buf[MAX_BUF];
  size_t buflen = MAX_BUF;

  if (!strncasecmp ("GET", (char *) data, 3))
    {
      printf ("Received GET\n");
      memset (buf, 0, MAX_BUF);
      ReadSensors (buf, &buflen);
      printf ("Sending: %s", buf);
      dtls_write (ctx, session, buf, buflen);
    }
  else
    {
      dtls_write (ctx, session, (uint8 *) INVALID_COMMAND,
		  strlen (INVALID_COMMAND));
    }
  return 0;
}

int
send_to_peer (struct dtls_context_t *ctx,
	      session_t * session, uint8 * data, size_t len)
{
  struct udp_socket *conn = (struct udp_socket *) dtls_get_app_data (ctx);

//  uip_ipaddr_copy (&conn->ripaddr, &session->addr);
//  conn->rport = session->port;
  printf("In send_to_peer()\n");
  PRINTF ("send to ");
  PRINT6ADDR (&session->addr);
  PRINTF (":%u\n", uip_ntohs(session->port));

  //uip_udp_packet_send (conn, data, len);
  /* Restore server connection to allow data from any node */
  //memset (&conn->ripaddr, 0, sizeof (conn->ripaddr));
  //memset (&conn->rport, 0, sizeof (conn->rport));
  return udp_socket_sendto(conn, data, len, &session->addr, session->port);
}

PROCESS (udp_server_process, "UDP server process");
AUTOSTART_PROCESSES (&udp_server_process);
/*---------------------------------------------------------------------------*/
#if 0
static void
dtls_handle_read (dtls_context_t * ctx)
{
  session_t session;

  if (uip_newdata ())
    {
      uip_ipaddr_copy (&session.addr, &UIP_IP_BUF->srcipaddr);
      session.port = UIP_UDP_BUF->srcport;
      session.size = sizeof (session.addr) + sizeof (session.port);

      dtls_handle_message (ctx, &session, uip_appdata, uip_datalen ());
    }
}
#endif
/*---------------------------------------------------------------------------*/

void
init ()
{
  static dtls_handler_t cb = {
    .write = send_to_peer,
    .read = read_from_peer,
    .event = NULL,
#ifdef DTLS_PSK
    .get_psk_info = get_psk_info,
#endif /* DTLS_PSK */
  };

  PRINTF ("DTLS server started\n");
  dtls_context = dtls_new_context (&server_conn);
  //PRINTF("write():%lu\n", (void *)cb.write);
  udp_socket_register (&server_conn, dtls_context, DtlsServerCB);
  udp_socket_bind (&server_conn, 20220);

  dtls_set_log_level (DTLS_LOG_DEBUG);

  //printf("1 dtls_context: %d\n", (int)dtls_context);

  printf("dtls_context: %d\n", (int)dtls_context);
  if (dtls_context)
    dtls_set_handler(dtls_context, &cb);
}

void
DtlsServerCB (struct udp_socket *sock,
	      void *apparg,
	      const uip_ipaddr_t * remoteaddr,
	      uint16_t remoteport,
	      const uip_ipaddr_t * localaddr,
	      uint16_t localport, const uint8_t * data, uint16_t datalen)
{
  session_t session;
  uint8_t databuf[MAX_BUF] = {0};

  PRINTF ("%u bytes received from ", datalen);
  PRINT6ADDR (localaddr);
  PRINTF(":%u.\n", remoteport);

  //Record remote endpoint.
  uip_ipaddr_copy (&session.addr, remoteaddr);
  session.port = remoteport;
  session.size = sizeof (session.addr) + sizeof (session.port);
  //Move data around...(FIXME)
  memcpy(databuf, data, datalen);
  printf("apparg:%d\n",(int)apparg);
  dtls_handle_message ((dtls_context_t *) apparg, &session, databuf, datalen);

  return;
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD (udp_server_process, ev, data)
{
  PROCESS_BEGIN ();

  dtls_init ();
  init ();

  if (!dtls_context)
    {
      dtls_emerg ("cannot create context\n");
      PROCESS_EXIT ();
    }

  while (1)
    {
      PROCESS_YIELD ();
    }

  PROCESS_END ();
}

/*---------------------------------------------------------------------------*/
