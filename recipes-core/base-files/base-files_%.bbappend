FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://environment file://env.sh"

do_compile_append() {
    # Setup root dir
    echo "HOME=${ROOT_HOME}" >> ${WORKDIR}/environmentc

    # Strip all comments
    sed "/^\s*#/d;s/\s*#[^\"']*$//" -i ${WORKDIR}/environment

    # Remove empty line
    sed '/^$/d' -i ${WORKDIR}/environment
}

do_install_append() {
    install -d ${D}${sysconfdir}/profile.d/
    install -m 0755 ${WORKDIR}/env.sh ${D}${sysconfdir}/profile.d/

    # install to /etc/
    install -d ${D}${sysconfdir}/
    install -m 0666 ${WORKDIR}/environment ${D}${sysconfdir}/environment
}

# This is machine specific package
PACKAGE_ARCH = "${MACHINE_ARCH}"
