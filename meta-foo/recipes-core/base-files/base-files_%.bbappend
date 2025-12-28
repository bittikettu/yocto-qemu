SUMMARY = "Update motd"
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://motd"

do_install:append() {
    install -m 0644 ${WORKDIR}/motd ${D}${sysconfdir}/motd
}
