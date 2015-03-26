SUMMARY = "A tool to read/write/update android boot images"
HOMEPAGE = "https://gitorious.org/ac100/abootimg"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS = "util-linux"

SRC_URI = "${DEBIAN_MIRROR}/main/a/${BPN}_${PV}.orig.tar.gz"
	
BBCLASSEXTEND = "native"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/abootimg ${D}${bindir}
    install -m 0755 ${S}/pack-initrd ${D}${bindir}/abootimg-pack-initrd
    install -m 0755 ${S}/unpack-initrd ${D}${bindir}/abootimg-unpack-initrd
}
