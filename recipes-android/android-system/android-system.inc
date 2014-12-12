DESCRIPTION = "Tiny Android system to leverage android hardware drivers through libhybris"
LICENSE = "Apache-2.0 & Properietary"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

inherit systemd useradd gettext

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = " \
        ${AOSP_HYBRIS_ANDROID_MANIFEST} \
        file://droid-hal-init.sh \
        file://android-system.service \
        file://65-android.rules \
"
S = "${WORKDIR}/repo"

# don't run package_do_shlibs on our packages as this will add a RDEPENDS from ${PN} to
# ${PN}-dbg
EXCLUDE_FROM_SHLIBS = "1"

# This is machine specific image
PACKAGE_ARCH = "${MACHINE_ARCH}"

# Create additional android users we need (need to have same UIDs as in android)
USERADD_PACKAGES = "${PN}"
USERADD_PARAM_${PN} = "-u 1000 -M system; -u 1001 -M radio; -u 1003 -M android_graphics; -u 1004 -M android_input;"

ANDROID_DEVICE ?= "${MACHINE}"

do_configure() {
    . build/envsetup.sh && lunch ${ANDROID_LUNCH_COMBO}
}

do_compile() {
    oe_runmake
}

# generate udev rules
do_compile_append () {
    _device="${ANDROID_DEVICE}"

    # Fix path for goldfish
    if [ "${MACHINE}" == "goldfisharmv7" ]; then
        _device="generic"
    fi
    if [ "${MACHINE}" == "goldfishx86" ]; then
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

    # Fix all init.*.rc loading path
    sed -i 's%import /init.%import /android/init.%g' ${S}/$_target/root/init.*

    # Change /sbin to /android/sbin
    sed -i 's%/sbin%/android/sbin%g' ${S}/$_target/root/init.*

    # Append function to generate bootdone flag
    echo "" >> ${S}/$_target/root/init.rc
    echo "service bootdone /system/bin/touch /dev/_android_boot_done" >> ${S}/$_target/root/init.rc
    echo "    class late_start" >> ${S}/$_target/root/init.rc
    echo "    oneshot" >> ${S}/$_target/root/init.rc
}

# TODO: generate android headers

do_install() {

    _device="${ANDROID_DEVICE}"

    # Fix path for goldfish
    if [ "${MACHINE}" == "goldfisharmv7" ]; then
        _device="generic"
    fi
    if [ "${MACHINE}" == "goldfishx86" ]; then
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
        # Avoid changing symlinks
        if [ ! -h ${D}/android/$file ] ; then
            chmod $mode ${D}/android/$file
        fi
    done < "${D}/android/filesystem_config.txt"

    # create symlink
    ln -sf /android/system  ${D}/system
    ln -sf /android/symbols ${D}/android/system/symbols
    ln -sf /system/vendor  ${D}/vendor

    install -d ${D}${sysconfdir}/udev/rules.d/
    install -m 0755 ${WORKDIR}/build/70-android.rules ${D}${sysconfdir}/udev/rules.d/
}

# Append for install service
do_install_append() {
        install -d ${D}${systemd_unitdir}/system
        install -m 0644 ${WORKDIR}/android-system.service ${D}${systemd_unitdir}/system

        install -d ${D}${sbindir}
        install -m 0755 ${WORKDIR}/droid-hal-init.sh ${D}${sbindir}

        install -d ${D}${sysconfdir}/udev/rules.d/
        install -m 0755 ${WORKDIR}/65-android.rules ${D}${sysconfdir}/udev/rules.d/
}

SYSTEMD_AUTO_ENABLE = "disable"
SYSTEMD_SERVICE_${PN} = "android-system.service"

# 400+ elf binaries without GNU_HASH and we cannot fix it
INSANE_SKIP_${PN} = "ldflags textrel dev-so"
INSANE_SKIP_${PN}-dbg = "ldflags textrel"

# Already stripped and we cannot fix it, this will hide a lot of
# warnings about files already stripped
INHIBIT_PACKAGE_STRIP = "1"
PACKAGES = "${PN}-dbg ${PN}"
FILES_${PN} = "/system /vendor /android ${sysconfdir} ${sbindir}"
FILES_${PN}-dbg = "/android/symbols"
