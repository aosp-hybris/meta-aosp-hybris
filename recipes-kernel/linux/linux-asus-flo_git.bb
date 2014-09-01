require recipes-kernel/linux/linux.inc

SECTION = "kernel"

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "flo"

DESCRIPTION = "Linux kernel for the Asus flo device (Nexus 7 2013)"

CMDLINE = "console=tty1"
KERNEL_RAM_BASE = "0x10008000"
RAMDISK_RAM_BASE = "0x11000000"
SECOND_RAM_BASE = "0x10f00000"
TAGS_RAM_BASE = "0x10000100"
BOOT_PARTITION = "/dev/mmcblk0p2"

inherit kernel_android

SRC_URI = " \
  git://github.com/shr-distribution/linux.git;protocol=git;branch=grouper/3.1/master \
  file://defconfig \
"

S = "${WORKDIR}/git/"

SRCREV = "9998c685b3282a8413c67218942bc200e6592c13"

KV = "3.1.10"
PV = "${KV}+gitr${SRCPV}"
# for bumping PR bump MACHINE_KERNEL_PR in the machine config
inherit machine_kernel_pr

# Workaround default -Werror setting and some warnings in kernel compilation
TARGET_CC_KERNEL_ARCH += " -Wno-error=unused-but-set-variable -Wno-error=array-bounds"
