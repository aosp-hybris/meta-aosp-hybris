DESCRIPTION = "Android header files extrace by libhybris"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = "git://github.com/aosp-hybris/android-headers;branch=${SRCBRANCH};protocol=git"
S = "${WORKDIR}/git"

PACKAGE_ARCH = "${MACHINE_ARCH}"
PROVIDES += "virtual/android-headers"

do_install() {
    install -d ${D}${includedir}/android
    cp -rv ${S}/* ${D}${includedir}/android/
    rm -rf ${D}${includedir}/android/android-headers.pc
    install -d ${D}${libdir}/pkgconfig
    install -m 0644 ${S}/android-headers.pc ${D}${libdir}/pkgconfig
}

FILES_${PN} = ""
FILES_${PN}-dev += "${libdir}/pkg-config"
ALLOW_EMPTY_${PN} = "1"
