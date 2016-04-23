CONTIKI_PROJECT = ecctiming ecdsatime
all: $(CONTIKI_PROJECT)

CONTIKI = ${CONTIKI_ROOT}

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

APPS+= tinydtls_timing tinydtls_timing/aes tinydtls_timing/sha2 tinydtls_timing/ecc

include $(CONTIKI)/Makefile.include
