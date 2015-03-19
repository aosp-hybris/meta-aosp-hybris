DESCRIPTION = "lxc aims to use these new functionnalities to provide an userspace container object"
SECTION = "console/utils"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=4fbd65380cdd255951079008b364516c"
PRIORITY = "optional"
PR = "r5"
DEPENDS = "libxml2 libcap"
RDEPENDS_${PN} = " \
                rsync \
                gzip \
                libcap-bin \
                bridge-utils \
                dnsmasq \
"
RDEPENDS_${PN}-ptest += "file make"

SRC_URI = "http://linuxcontainers.org/downloads/${BPN}-${PV}.tar.gz \
        file://lxc-1.0.0-disable-udhcp-from-busybox-template.patch \
        file://runtest.patch \
        file://run-ptest \
        file://automake-ensure-VPATH-builds-work-correctly.patch \
        "
SRC_URI[md5sum] = "55873b1411a606397309aa6c4c4263b3"
SRC_URI[sha256sum] = "3538c16083d0265fe46ba2d9e33054550f0d2dc1fb21b729b456c31769947e96"

S = "${WORKDIR}/${BPN}-${PV}"

# Let's not configure for the host distro.
#
PTEST_CONF = "${@base_contains('DISTRO_FEATURES', 'ptest', '--enable-tests', '', d)}"
EXTRA_OECONF += "--with-distro=${DISTRO} ${PTEST_CONF}"

PACKAGECONFIG ??= ""
PACKAGECONFIG[doc] = "--enable-doc,--disable-doc,,"
PACKAGECONFIG[api-docs] = "--enable-api-docs,--disable-api-docs,--disable-api-docs,"
PACKAGECONFIG[rpath] = "--enable-rpath,--disable-rpath,,"
PACKAGECONFIG[apparmour] = "--enable-apparmor,--disable-apparmor,apparmor,apparmor"

inherit autotools pkgconfig ptest

FILES_${PN}-doc = "${mandir} ${infodir} ${docdir}"
FILES_${PN}-dbg += "${libexecdir}/lxc/.debug"

PRIVATE_LIBS_${PN}-ptest = "liblxc.so.1"

do_install_append() {
        # The /var/cache/lxc directory created by the Makefile
        # is wiped out in volatile, we need to create this at boot.
        rm -rf ${D}${localstatedir}/cache
        install -d ${D}${sysconfdir}/default/volatiles
        echo "d root root 0755 ${localstatedir}/cache/lxc none" \
             > ${D}${sysconfdir}/default/volatiles/99_lxc
}

EXTRA_OEMAKE += "TEST_DIR=${D}${PTEST_PATH}/src/tests"

do_install_ptest() {
        oe_runmake -C src/tests install-ptest
}

pkg_postinst_${PN}() {
        if [ -z "$D" ] && [ -e /etc/init.d/populate-volatile.sh ] ; then
                /etc/init.d/populate-volatile.sh update
        fi
}
