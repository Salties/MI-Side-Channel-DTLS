#CONTIKI_PROJECT = dtls-server dtls-client
#CONTIKI_PROJECT = aestest_fixed aestest_random
CONTIKI_PROJECT = helloworld
all: $(CONTIKI_PROJECT)

CONTIKI = $(CONTIKI_ROOT)
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls
#APPS += servreg-hack

#Uncomment to enable null mac and rdc driver
#CFLAGS += -DNULL_DRIVER

CFLAGS += -DTARGET_NAME=\"$(TARGET)\"
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
CFLAGS += -DVERBOSE_AESTEST

include $(CONTIKI)/Makefile.include
