include tools/setup.mk

TARGET := target
BUILD_DIR := ./build
BOARD ?= metro_m0

include board/$(BOARD).mk
include tools/defaults.mk

INC_DIRS += \

SYS_INC_DIRS += \

SRC_FILES += \

SRC_DIRS += \
  src \

include lib_asf.mk
include lib_rtt.mk
include lib/tiny/lib_tiny.mk
include lib/tiny-devices/lib_tiny-devices.mk
include lib_hardware.mk

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	@$(SIZE) $<

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"

include tools/tools.mk
include docs.mk

.PHONY: rtt-client
rtt-client:
	@{ JLinkGDBServer -NoGui 1 -nohalt -singlerun -device $(JLINK_DEVICE) -if SWD -autoconnect 1 -speed 4000 & } && trap 'sleep 0.5' INT && JLinkRTTClient

.PHONY: run
run:
	@$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST)) upload
	@$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST)) rtt-client
