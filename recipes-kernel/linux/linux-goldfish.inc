require recipes-kernel/linux/linux.inc
require setup-defconfig.inc

DESCRIPTION = "Linux kernel for the Android emulator device (goldfish)"
SECTION = "kernel"

DEPENDS += "libgcc"

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "goldfisharm*|goldfishx86"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = "git://github.com/aosp-hybris/android_kernel_goldfish.git;branch=${SRCBRANCH};protocol=https"
# SRC_URI = "git://android.googlesource.com/kernel/goldfish;branch=${SRCBRANCH};protocol=https"

S = "${WORKDIR}/git"

do_configure_prepend() {
    if [ "${MACHINE}" == "goldfisharmv7" ]; then
        echo "use-kernel-config=goldfish_armv7_defconfig" > ${WORKDIR}/defconfig
    elif [ "${MACHINE}" == "goldfishx86" ]; then
        echo "use-kernel-config=goldfish_defconfig" > ${WORKDIR}/defconfig
    else
        bberror "no suitable linux-goldfish config for ${MACHINE}"
    fi
}
