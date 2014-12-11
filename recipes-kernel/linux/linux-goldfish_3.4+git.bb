require ${BPN}.inc

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "emulator"

SRCBRANCH = "android-goldfish-3.4"

SRCREV ?= "${AUTOREV}"

LINUX_VERSION = "3.4"
PV = "${LINUX_VERSION}+gitr${SRCPV}"

#PNBLACKLIST[linux-goldfish] = "Failed to emulation libEGL_emulation.so in android-4.2.2_r1"
