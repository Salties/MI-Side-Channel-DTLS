CONTIKI_PROJECT = dtls-client dtls-server
all: $(CONTIKI_PROJECT)

CONTIKI = ../contiki
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

APPS += tinydtls tinydtls/aes tinydtls/sha2 tinydtls/ecc

include $(CONTIKI)/Makefile.include
