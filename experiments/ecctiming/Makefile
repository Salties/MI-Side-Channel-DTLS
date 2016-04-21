CONTIKI_PROJECT = ecctiming
all: $(CONTIKI_PROJECT)

CONTIKI = ${CONTIKI_ROOT}

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

APPS+= tinydtls tinydtls/aes tinydtls/sha2 tinydtls/ecc

include $(CONTIKI)/Makefile.include
