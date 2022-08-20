$(call create_lib_with_defaults,rtt)

rtt_DEFINES := $(DEFINES)

rtt_CFLAGS += \
  -Wno-cast-qual

rtt_INC_DIRS := \
  lib/rtt/Config \
  lib/rtt/inc \

rtt_SRC_FILES := \

rtt_SRC_DIRS := \
  lib/rtt/src \

INC_DIRS += \
  lib/rtt/inc \
