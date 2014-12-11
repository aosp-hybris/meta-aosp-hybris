require recipes-kernel/linux/linux.inc

DESCRIPTION = "Linux kernel for the Android emulator device (goldfish)"
SECTION = "kernel"

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "emulator|emulator-x86"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = " \
  git://github.com/aosp-hybris/android_kernel_goldfish.git;branch=${SRCBRANCH};protocol=git \
  file://defconfig \
"

S = "${WORKDIR}/git"

KV ?= "3.4"
PV = "${KV}+gitr${SRCPV}"
