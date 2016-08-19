CONTIKI_PROJECT = example-powertrace
APPS+=powertrace
all: $(CONTIKI_PROJECT)

CONTIKI=$(CONTIKI_ROOT)
CONTIKI_WITH_IPV6 = 1
include $(CONTIKI)/Makefile.include

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
