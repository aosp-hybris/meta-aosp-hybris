inherit core-image

LICENSE = "MIT"
PACKAGES = ""
IMAGE_LINGUAS = " "

INHIBIT_DEFAULT_DEPS = "1"

# Always add package-management features
IMAGE_FEATURES_prepend = " package-management "

# Need to inherit image late to make sure variable be modified by this class
inherit image
