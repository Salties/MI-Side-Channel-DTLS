CONTIKI_PROJECT = pingload
all: $(CONTIKI_PROJECT)

CONTIKI = $(CONTIKI_ROOT)
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

#Uncomment to enable led indicator.
#CFLAGS += -DLED_INDICATOR

#Uncomment to use seconds for payload and idle instead of clock ticks.
#CFLAGS += -DSECOND

#Uncomment to enable null_driver instead of ContikiMAC.
#CFLAGS += -DNULL_DRIVER

#Uncomment to use randomised payload period.
#CFLAGS += -DRAND_PAYLOAD

#Uncomment to use randomised idle period.
#CFLAGS += -DRAND_IDLE

include $(CONTIKI)/Makefile.include
