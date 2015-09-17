CONTIKI_PROJECT = dtls-server dtls-client
all: $(CONTIKI_PROJECT)

CONTIKI = $(CONTIKI_ROOT)
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

CFLAGS += -DPLATFORM_$(TARGET)
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

include $(CONTIKI)/Makefile.include
