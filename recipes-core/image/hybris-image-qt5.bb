DESCRIPTION = "libhybris image with qt5 libs."

inherit core-image

IMAGE_INSTALL += "libhybris"

IMAGE_INSTALL += "cinematicexperience"

IMAGE_INSTALL += " \
        qtbase \
        qtbase-fonts \
        qtbase-plugins \
        qtdeclarative \
        qtdeclarative-qmlplugins \
        qtdeclarative-tools"

# TODO: Add basic android-system-image and config

IMAGE_INSTALL += "qt5-qpa-hwcomposer-plugin"
