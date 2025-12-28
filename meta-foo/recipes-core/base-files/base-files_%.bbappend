SUMMARY = "Update some own style stuff"
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://motd file://my-profile.sh"

do_install:append() {
    install -m 0644 ${WORKDIR}/motd ${D}${sysconfdir}/motd
    install -d ${D}${sysconfdir}/profile.d
    install -m 0644 ${WORKDIR}/my-profile.sh \
        ${D}${sysconfdir}/profile.d/my-profile.sh
}
