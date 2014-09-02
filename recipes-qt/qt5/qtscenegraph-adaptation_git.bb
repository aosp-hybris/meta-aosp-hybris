DESCRIPTION = ""
LICENSE = "LGPL-2.1"
LIC_FILES_CHKSUM = "file://LICENSE.GPL;md5=d32239bcb673463ab874e80d47fae504"

PV = "5.3+gitr${SRCPV}"

DEPENDS = "qtbase qtdeclarative libhybris virtual/android-headers "

# We need to be ${MACHINE_ARCH} as we need to compile the source against a specific
# Android version we select per machine
PACKAGE_ARCH = "${MACHINE_ARCH}"

SRCBRANCH ??= "master"
SRCREV ?= "${AUTOREV}"

SRC_URI = "git://github.com/aosp-hybris/qtscenegraph-adaptation.git;branch=${SRCBRANCH};protocol=git"
S = "${WORKDIR}/git"

inherit qmake5

# Set path of qt5 headers as qmake5_base.bbclass sets this to just ${includedir} but
# actually it is ${includedir}/qt5
OE_QMAKE_PATH_HEADERS = "${OE_QMAKE_PATH_QT_HEADERS}"

# TODO: Make user can customize this
EXTRA_QMAKEVARS_PRE += "CONFIG+=animationdriver"
EXTRA_QMAKEVARS_PRE += "CONFIG+=programbinary"
EXTRA_QMAKEVARS_PRE += "CONFIG+=eglgralloctexture"
EXTRA_QMAKEVARS_PRE += "INCLUDEPATH+=${STAGING_DIR_TARGET}${includedir}/android"

FILES_${PN} += "${libdir}/qt5"
FILES_${PN}-dbg += "${libdir}/qt5/*/*/.debug"
