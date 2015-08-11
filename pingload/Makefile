CONTIKI_PROJECT = pingload
CONTIKI=../contiki
all: $(CONTIKI_PROJECT)

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CONTIKI_WITH_IPV6 = 1
#Enable LED indicator
#CFLAGS+= -DLED_INDICATOR

include $(CONTIKI)/Makefile.include
