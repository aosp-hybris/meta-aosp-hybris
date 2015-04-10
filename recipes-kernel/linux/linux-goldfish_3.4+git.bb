require ${BPN}.inc

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "goldfishx86|goldfisharmv7"

SRCBRANCH = "android-goldfish-3.4"

SRCREV ?= "${AUTOREV}"

LINUX_VERSION = "3.4"
PV = "${LINUX_VERSION}+gitr${SRCPV}"

