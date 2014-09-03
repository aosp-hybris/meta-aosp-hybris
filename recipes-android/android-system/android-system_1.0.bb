DESCRIPTION = "System configuration and startup scripts for the Android compatibility layer"
LICENSE = "GPL-3.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0;md5=c79ff39f19dfec6d293b95dea7b07891"

PACKAGE_ARCH = "${MACHINE_ARCH}"

# For running the container we're using lxc (>= 1.0 required)
RDEPENDS_${PN} += "lxc \
        lxc-android-config \
        virtual/android-system-image \
        "
inherit systemd useradd

SRC_URI = " \
        file://android-system.service \
        file://wait-for-android.sh \
        file://android-chroot \
        "

# Create additional android users we need (need to have same UIDs as in android)
USERADD_PACKAGES = "${PN}"
USERADD_PARAM_${PN} = "-u 1000 -M system; -u 1001 -M radio; -u 1003 -M graphics; -u 1004 -M input;"

do_install() {
        install -d ${D}${systemd_unitdir}/system
        install -m 0644 ${WORKDIR}/android-system.service ${D}${systemd_unitdir}/system

        install -d ${D}${bindir}
        install -m 0755 ${WORKDIR}/android-chroot ${D}${bindir}
        install -m 0755 ${WORKDIR}/wait-for-android.sh ${D}${bindir}
}

SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_SERVICE_${PN} = "android-system.service"
