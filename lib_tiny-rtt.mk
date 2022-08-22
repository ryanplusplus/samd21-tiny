$(call create_lib_with_defaults,tiny-rtt)

tiny-rtt_DEFINES := $(DEFINES)

tiny-rtt_CFLAGS += \
  -Wno-cast-qual

tiny-rtt_INC_DIRS := \
  lib/tiny-rtt/Config \
  lib/tiny-rtt/inc \

tiny-rtt_SRC_FILES := \

tiny-rtt_SRC_DIRS := \
  lib/tiny-rtt/src \

INC_DIRS += \
  lib/tiny-rtt/inc \
