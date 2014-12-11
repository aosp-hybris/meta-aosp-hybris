require recipes-kernel/linux/linux.inc
require setup-defconfig.inc

DESCRIPTION = "Linux kernel for the Android emulator device (goldfish)"
SECTION = "kernel"

# Mark archs/machines that this kernel supports
COMPATIBLE_MACHINE = "emulator|emulator-x86"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

# SRC_URI = "git://github.com/aosp-hybris/android_kernel_goldfish.git;branch=${SRCBRANCH};protocol=git"
SRC_URI = "git://android.googlesource.com/kernel/goldfish;branch=${SRCBRANCH};protocol=git"

S = "${WORKDIR}/git"

do_configure_prepend() {
    if [ "${MACHINE}" == "emulator" ]; then
        echo "use-kernel-config=goldfish_armv7_defconfig" > ${WORKDIR}/defconfig
    elif [ "${MACHINE}" == "emulator-x86" ]; then
        echo "use-kernel-config=goldfish_defconfig" > ${WORKDIR}/defconfig
    else
        bberror "no suitable linux-goldfish config for ${MACHINE}"
    fi
}
