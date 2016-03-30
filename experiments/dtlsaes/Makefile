CONTIKI_PROJECT = aestest_fixed aestest_random

all: $(CONTIKI_PROJECT)

CONTIKI = $(CONTIKI_ROOT)
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls
#APPS += servreg-hack

#Uncomment to enable null mac and rdc driver
#CFLAGS += -DNULL_DRIVER

CFLAGS += -DTARGET_NAME=\"$(TARGET)\"
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CFLAGS += -DVERBOSE_AESTEST -O0

include $(CONTIKI)/Makefile.include
