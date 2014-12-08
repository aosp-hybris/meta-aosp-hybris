DESCRIPTION = "Android emulator with goldfish pipe."
LICENSE = "Apache-2.0 & Properietary"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRCBRANCH ??= "hybris-4.2.2_r1"
SRCREV ?= "${AUTOREV}"

SRC_URI = "repo://github.com/aosp-hybris/platform_manifest.git;branch=${SRCBRANCH};protocol=git"

S = "${WORKDIR}"

do_configure() {
    . build/envsetup.sh && lunch full-eng
}

do_compile() {
    oe_runmake
}

do_install () {
    _target=${datadir}/android/emulator
    _bindir=${_target}/bin
    _libdir=${_target}/lib
    _datadir=${_target}/usr

    install -d ${D}${_bindir}
    cp -vrf ${S}/out/host/linux-x86/bin/emulator* ${D}${_bindir}
    install -v -m -755 ${S}/out/host/linux-x86/bin/simg2img ${D}${_bindir}

    install -d ${D}${_libdir}
    cp -vrf ${S}/out/host/linux-x86/lib/* ${D}${_libdir}

    install -d ${D}${_datadir}
    cp -vrf ${S}/out/host/linux-x86/usr/* ${D}${_datadir}
    cp -vrf ${S}/development/tools/emulator/skins ${D}${_datadir}
}

BBCLASSEXTEND = "nativesdk"
