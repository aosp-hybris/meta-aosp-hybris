FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

PR = "r2"

SRC_URI += "file://change-android-group-ids.patch"
