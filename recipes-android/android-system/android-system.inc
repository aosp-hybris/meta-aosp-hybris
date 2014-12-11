DESCRIPTION = "System configuration and startup scripts for the Android compatibility layer"
LICENSE = "GPL-3.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-3.0;md5=c79ff39f19dfec6d293b95dea7b07891"

PACKAGE_ARCH = "${MACHINE_ARCH}"

RDEPENDS_${PN} += "virtual/android-system-image"

inherit systemd useradd

SRC_URI = " \
        file://droid-hal-init.sh \
        file://android-system.service \
        file://65-android.rules \
"

# Create additional android users we need (need to have same UIDs as in android)
USERADD_PACKAGES = "${PN}"
USERADD_PARAM_${PN} = "-u 1000 -M system; -u 1001 -M radio; -u 1003 -M android_graphics; -u 1004 -M android_input;"

do_install() {
        install -d ${D}${systemd_unitdir}/system
        install -m 0644 ${WORKDIR}/android-system.service ${D}${systemd_unitdir}/system

        install -d ${D}${bindir}
        install -m 0755 ${WORKDIR}/droid-hal-init.sh ${D}${sbindir}

        install -d ${D}${sysconfdir}/udev/rules.d/
        install -m 0755 ${WORKDIR}/65-android.rules ${D}${sysconfdir}/udev/rules.d/
}

SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_SERVICE_${PN} = "android-system.service"

FILES_${PN} += "${sbindir} ${bindir}"
