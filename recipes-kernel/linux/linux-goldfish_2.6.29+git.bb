require ${BPN}.inc

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "emulator"

SRCBRANCH = "android-goldfish-2.6.29"
SRCREV ?= "${AUTOREV}"

LINUX_VERSION = "2.6.29"
PV = "${LINUX_VERSION}+gitr${SRCPV}"
