SUMMARY = "raylib is a simple and easy-to-use library to enjoy videogames programming"
HOMEPAGE = "https://github.com/raysan5/raylib"
LICENSE = "zlib-acknowledgement"
LIC_FILES_CHKSUM = "file://LICENSE;md5=e5c04526673eda16f837e05cb1177261"

SRC_URI = "git://github.com/raysan5/raylib.git;protocol=https"
SRCREV = "c1ab645ca298a2801097931d1079b10ff7eb9df8"

S = "${WORKDIR}/git"

DEPENDS = "\
    glfw \
    wayland-native \
    wayland \
    wayland-protocols \
    libxkbcommon \
    mesa \
"

EXTRA_OECMAKE = "-DPLATFORM=Desktop -DUSE_WAYLAND=ON -DUSE_X11=OFF -DGLFW_BUILD_WAYLAND=ON -DGLFW_BUILD_X11=OFF -DBUILD_SHARED_LIBS=ON -DOPENGL_VERSION=2.1"

inherit cmake pkgconfig
