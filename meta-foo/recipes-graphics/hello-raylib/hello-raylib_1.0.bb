SUMMARY = "Simple raylib application"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "raylib"

SRC_URI = "file://hello-raylib.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${LDFLAGS} hello-raylib.c -o hello-raylib -lraylib -lm -lpthread -ldl -lrt -lGLESv2 -lEGL -ldrm -lgbm -lwayland-client
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 hello-raylib ${D}${bindir}
}
