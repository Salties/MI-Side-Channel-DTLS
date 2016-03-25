all: broadcast-example unicast-sender unicast-receiver aestest_fixed aestest_random
#APPS=servreg-hack
CONTIKI=$(CONTIKI_ROOT)

APPS+=servreg-hack

CONTIKI_WITH_IPV6 = 1
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CFLAGS += -DTARGET_NAME=\"${TARGET}\"

#Enable LLSEC
CFLAGS += -DENABLE_LLSEC
MODULES += core/net/llsec/noncoresec

#Verbose output for aestests
#CFLAGS += -DVERBOSE_AESTEST

#Number of Rounds for aestiming.
CFLAGS += -DNROUND=200

#Number of Sample for aestiming.
CFLAGS += -DNSAMPLE=1000

include $(CONTIKI)/Makefile.include
