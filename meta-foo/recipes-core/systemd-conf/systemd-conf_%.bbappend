FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://wlan.network file://can.network"

FILES:${PN} += "${sysconfdir}/systemd/network/wlan.network ${sysconfdir}/systemd/network/can.network"

do_install:append() {
    install -d ${D}${sysconfdir}/systemd/network
    install -m 0644 ${WORKDIR}/wlan.network ${D}${sysconfdir}/systemd/network
    install -m 0644 ${WORKDIR}/can.network ${D}${sysconfdir}/systemd/network
}