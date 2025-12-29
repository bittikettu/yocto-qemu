SUMMARY = "Fake Hardware Clock"
DESCRIPTION = "Saves and restores time on boot/shutdown for devices without RTC"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://fake-hwclock.sh \
    file://fake-hwclock.service \
"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE:${PN} = "fake-hwclock.service"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/fake-hwclock.sh ${D}${bindir}/fake-hwclock.sh

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/fake-hwclock.service ${D}${systemd_system_unitdir}/
}

FILES:${PN} += "${bindir}/fake-hwclock.sh ${systemd_system_unitdir}/fake-hwclock.service"
