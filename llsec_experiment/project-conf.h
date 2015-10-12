#ifndef __PROJECT_CONF_H__
#define __PROJECT_CONF_H__

#define UIP_CONF_TCP 0

//#define ENABLE_LLSEC

//RDC driver
//#undef NETSTACK_CONF_RDC
//#define NETSTACK_CONF_RDC nullrdc_driver
//#define NETSTACK_CONF_RDC contikimac_driver
//#define NETSTACK_CONF_RDC sicslowmac_driver
//#define NETSTACK_CONF_RDC cxmac_driver

//MAC driver
//#undef NETSTACK_CONF_MAC
//#define NETSTACK_CONF_MAC nullmac_driver

//#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE 8

#ifdef ENABLE_LLSEC
#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC noncoresec_driver
#undef IEEE802154_CONF_SECURITY_LEVEL
#define IEEE802154_CONF_SECURITY_LEVEL 2

#define NONCORESEC_CONF_KEY {\
	0x00 , 0x01 , 0x02 , 0x03 ,\
	0x04 , 0x05 , 0x06 , 0x07 ,\
	0x08 , 0x09 , 0x0A , 0x0B ,\
	0x0C , 0x0D , 0x0E , 0x0F \
}
#endif

#endif
