DESCRIPTION = "Configuration to fire up android container."
SECTION = "console/utils"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

RDEPENDS_${PN} = "lxc virtual/android-system-image"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"
SRC_URI = "git://github.com/aosp-hybris/lxc-android-config;branch=${SRCBRANCH};protocol=git"
S = "${WORKDIR}/git"

do_install() {
    install -d ${D}/usr/lib/lxc-android-config
    cp -rf ${S}/usr/lib/lxc-android-config/* ${D}/usr/lib/lxc-android-config/

    install -d ${D}/var/lib/lxc/android/rootfs
}
