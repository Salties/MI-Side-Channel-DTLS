all: broadcast-example unicast-sender unicast-receiver
#APPS=servreg-hack
CONTIKI=$(CONTIKI_ROOT)

APPS+=servreg-hack

CONTIKI_WITH_IPV6 = 1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include $(CONTIKI)/Makefile.include
