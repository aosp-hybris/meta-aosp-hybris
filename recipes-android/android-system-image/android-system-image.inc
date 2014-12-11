DESCRIPTION = "Tiny Android system to leverage android hardware drivers through libhybris"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit gettext

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = "${AOSP_HYBRIS_ANDROID_MANIFEST}"
S = "${WORKDIR}/repo"

# don't run package_do_shlibs on our packages as this will add a RDEPENDS from ${PN} to
# ${PN}-dbg
EXCLUDE_FROM_SHLIBS = "1"

# This is machine specific image
PACKAGE_ARCH = "${MACHINE_ARCH}"

ANDROID_DEVICE ?= "${MACHINE}"

RPROVIDES_${PN} += "virtual/android-system-image"

do_configure() {
    . build/envsetup.sh && lunch ${ANDROID_LUNCH_COMBO}
}

do_compile() {
    oe_runmake
}

# generate udev rules
do_compile_append () {
    _device="${ANDROID_DEVICE}"

    # Fix path for emulator
    if [ "${MACHINE}" == "emulator" ]; then
        _device="generic"
    fi
    if [ "${MACHINE}" == "emulator-x86" ]; then
        _device="generic_x86"
    fi

    _target=out/target/product/$_device

    # Generate 70-android.rules from ueventd.rc and ueventd.*.rc
    mkdir -p ${WORKDIR}/build
    echo > ${WORKDIR}/build/70-android.rules

    grep -v -E "(^\#|^$|^/dev/[^\w]*/[^\w]*|/sys)" ${S}/$_target/root/ueventd.rc | \
        sed "s|/dev/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

    grep -E "(^/dev/block/[^\w]*)" ${S}/$_target/root/ueventd.rc | \
        sed "s|/dev/block/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

    if [ -e ${S}/$_target/root/ueventd.$ANDROID_DEVICE.rc ]; then
        echo "Your ANDROID_DEVICE is \"${ANDROID_DEVICE}\", now building for udev rules."

        grep -v -E "(^\#|^$|^/dev/[^\w]*/[^\w]*|/sys)" ${S}$_target/root/ueventd.$ANDROID_DEVICE.rc | \
            sed "s|/dev/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules

        grep -E "(^/dev/block/[^\w]*)" ${S}/$_target/root/ueventd.$ANDROID_DEVICE.rc | \
            sed "s|/dev/block/||g" | awk '{ print "ACTION==\"add\" KERNEL==\"" $1 "\", MODE=\"" $2 "\", GROUP=\"" $4 "\", OWNER=\"" $3 "\"" }' >> ${WORKDIR}/build/70-android.rules
    else
        bbwarn "No extra ueventd.xxx.rc found, make sure you define ANDROID_DEVICE variable."
    fi

    # Add ro.hybris.bootinternal=1 to prevent android's console steal
    # hardware prompt
    echo "#\n\# AOSP_HYBRIS_PROPERTIES\n#\n" >> ${S}/$_target/system/build.prop
    echo "ro.hybris.bootinternal=1" >> ${S}/$_target/system/build.prop
}

# TODO: generate android headers

do_install() {

    _device="${ANDROID_DEVICE}"

    # Fix path for emulator
    if [ "${MACHINE}" == "emulator" ]; then
        _device="generic"
    fi
    if [ "${MACHINE}" == "emulator-x86" ]; then
        _device="generic_x86"
    fi

    _target=out/target/product/$_device

    install -d ${D}/android

    rsync -ar ${S}/$_target/root/* ${D}/android
    rsync -ar ${S}/$_target/system ${D}/android
    rsync -ar ${S}/$_target/symbols ${D}/android

    # FIXME: move to compile ?
    # generate filesystem_config.txt
    # we need this to fix android permission
    du -a ${S}/$_target/system/ | awk '{print $2}' |
        sed "s%$(pwd)/$_target/%%g" | out/host/linux-x86/bin/fs_config > ${D}/android/filesystem_config.txt

    # Fixup permissions of the android binaries in /system
    # FIXME: how about user and group ?
    while read line
    do
        file=`echo $line | cut -d' ' -f 1`
        user=`echo $line | cut -d' ' -f 2`
        group=`echo $line | cut -d' ' -f 3`
        mode=`echo $line | cut -d' ' -f 4`
        chown -h $user:$group ${D}/android/$file
        echo "${D}/android/$file"
        # Avoid changing symlinks
        if [ ! -h ${D}/android/$file ] ; then
            chmod $mode ${D}/android/$file
        fi
    done < "${D}/android/filesystem_config.txt"

    # create symlink
    ln -sf /android/system  ${D}/system
    ln -sf /android/symbols ${D}/android/system/symbols
    ln -sf /system/vendor  ${D}/vendor

    # FIXME: move to compile ?
    # fix all init.*.rc loading path
    sed -i 's%import /init.%import /android/init.%g' ${D}/android/init.*

    install -d ${D}${sysconfdir}/udev/rules.d/
    install -m 0755 ${WORKDIR}/build/70-android.rules ${D}${sysconfdir}/udev/rules.d/
}

# 400+ elf binaries without GNU_HASH and we cannot fix it
INSANE_SKIP_${PN} = "ldflags textrel dev-so"
INSANE_SKIP_${PN}-dbg = "ldflags textrel"

# Already stripped and we cannot fix it, this will hide a lot of
# warnings about files already stripped
INHIBIT_PACKAGE_STRIP = "1"
PACKAGES = "${PN}-dbg ${PN}"
FILES_${PN} = "/system /vendor /android ${sysconfdir}"
FILES_${PN}-dbg = "/android/symbols"
