DESCRIPTION = "hybris image with qt5 libs."

inherit hybris-image

# Use runlevel 5 as default
SYSTEMD_DEFAULT_TARGET = "graphical.target"

IMAGE_INSTALL += "libhybris"

IMAGE_INSTALL += "cinematicexperience"

IMAGE_INSTALL += " \
        qtbase \
        qtbase-fonts \
        qtbase-plugins \
        qtdeclarative \
        qtdeclarative-qmlplugins \
        qtdeclarative-tools \
        qtscript \
        qtserialport \
        qtimageformats  \
        qtgraphicaleffects-qmlplugins \
        qtsvg \
        qtxmlpatterns \
        qtwayland \
        qtwayland-plugins \
        qt5-qpa-hwcomposer-plugin \
"

# TODO: Add basic android-system-image and config

REQUIRED_DISTRO_FEATURES = "wayland"
