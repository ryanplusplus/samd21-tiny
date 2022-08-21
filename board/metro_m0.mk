CPU := cortex-m0
LINKER_SCRIPT := lib/asf4/samd21/gcc/gcc/samd21g18a_flash.ld

SVD := lib/svd/ATSAMD21G18A.svd
OPENOCD_CONFIG := openocd/samd21g18.cfg

DEBUG_ADAPTER ?= jlink
JLINK_DEVICE := ATSAMD21G18
BLACK_MAGIC_PORT ?= /dev/ttyACM0
BLACK_MAGIC_POWER_TARGET ?= N

INC_DIRS += \
  src/board/metro_m0

DEFINES += \
  __SAMD21G18A__ \
