CONTIKI_PROJECT = aestest_fixed aestest_random

all: $(CONTIKI_PROJECT)

CONTIKI = $(CONTIKI_ROOT)
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

CFLAGS += -DTARGET_NAME=\"$(TARGET)\"
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#Use verbose outpu.
CFLAGS += -DVERBOSE_AESTEST
#Calbrate the timing framework.
#CFLAGS += -DCALIBRATE_FRAMEWORK

include $(CONTIKI)/Makefile.include
