DESCRIPTION = "Tiny Android system to leverage android hardware drivers through libhybris"
LICENSE = "Apache-2.0 & Properietary"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

# don't run package_do_shlibs on our packages as this will add a RDEPENDS from ${PN} to
# ${PN}-dbg
EXCLUDE_FROM_SHLIBS = "1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

RPROVIDES_${PN} += "virtual/android-system-image"

ANDROID_DEVICE ??= "${MACHINE}"

S = "${WORKDIR}"

do_compile () {
    echo "Your ANDROID_DEVICE is \"${ANDROID_DEVICE}\", now building for udev rules."

    mkdir -p ${WORKDIR}/build
    grep -v -E "(^\#|^$|^/dev/[^\w]*/[^\w]*|/sys)" ${S}/root/ueventd.rc | \
        sed "s|/dev/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

    grep -v -E "(^\#|^$|^/dev/[^\w]*/[^\w]*|/sys)" ${S}/ueventd.$ANDROID_DEVICE.rc | \
        sed "s|/dev/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

    grep -E "(^/dev/block/[^\w]*)" ${S}/ueventd.rc | \
        sed "s|/dev/block/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

    grep -E "(^/dev/block/[^\w]*)" $${S}/ueventd.$ANDROID_DEVICE.rc | \
        sed "s|/dev/block/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules
}

do_install() {
    # TODO:
    # /android
    # /android/system
    # /android/init.*
    # /android/symbols
    # /android/vendors
    # /system  <-- symlink
    # /system/symbols  <-- symlink
    # /vendor  <- sumlink

    install -d ${D}/android
    cp -rpv ${S}/* ${D}/android
    cp -rpv ${S}/root/* ${D}/android

    rm -rf ${D}/android/root

    # create symlink
    ln -sf /android/system  ${D}/system
    ln -sf /android/symbols ${D}/android/system/symbols
    ln -sf /system/vendor  ${D}/vendor

    install -d ${D}${sysconfdir}/udev/rules.d/
    install -m 0755 ${WORKDIR}/build/70-android.rules ${D}${sysconfdir}/udev/rules.d/
}

# 400+ elf binaries without GNU_HASH and we cannot fix it
INSANE_SKIP_${PN} = "ldflags textrel dev-so"
INSANE_SKIP_${PN}-dbg = "ldflags textrel"
# Already stripped and we cannot fix it, this will hide a lot of warnings about files already stripped
INHIBIT_PACKAGE_STRIP = "1"
PACKAGES = "${PN}-dbg ${PN}"
FILES_${PN}-dbg = "/android/symbols"
FILES_${PN} = "/system /vendor /android ${sysconfdir}"
