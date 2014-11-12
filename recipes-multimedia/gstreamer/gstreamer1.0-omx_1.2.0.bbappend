FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://0001_Add_libhybris_support.patch"

# This is machine specific package
PACKAGE_ARCH = "${MACHINE_ARCH}"
