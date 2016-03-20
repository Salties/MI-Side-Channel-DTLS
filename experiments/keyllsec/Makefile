all: broadcast-example unicast-sender unicast-receiver aestest
#APPS=servreg-hack
CONTIKI=$(CONTIKI_ROOT)

APPS+=servreg-hack

CONTIKI_WITH_IPV6 = 1
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

#Enable LLSEC
CFLAGS += -DENABLE_LLSEC
MODULES += core/net/llsec/noncoresec

include $(CONTIKI)/Makefile.include
