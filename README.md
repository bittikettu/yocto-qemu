# QEMU Jokto Project

This project is a learning platform for Yocto and Kas, demonstrating how to add custom layers, create recipes, and modify existing system components.

Google Antigravity + NotebookLM with generated podcasts based on multiple Youtube videos and blogposts have been used to create this project.

Huge thanks to:
- https://github.com/YoseliSAS/yocto-blog-posts
- https://www.yoseli.org/building-a-robust-raspberry-pi-4-system-with-kas-yocto-mender-and-squashfs
- https://www.youtube.com/watch?v=8M8U1EgnUVw
- https://embeddeduse.com/2020/05/26/qt-embedded-systems-1-build-linux-image-with-yocto/
- https://www.youtube.com/@LeonAnavi

## meta-foo Layer

The `meta-foo` layer is a custom layer designed to demonstrate:
- **Layer Creation**: How to structure a custom Yocto layer.
- **Recipe Customization**: Using `bbappend` files to modify existing recipes (e.g., `base-files`).
- **File Overrides**: Replacing system files like `/etc/motd` with custom versions using proper directory precedence (e.g., `recipes-core/base-files/files/poky/`).

### Key Features
- **Custom MOTD**: Replaces the default system message of the day (motd) with a custom "Otter Side" message.
- **Fake HW Clock**: Saves and restores time on boot/shutdown for devices without RTC (e.g. Raspberry Pi).
- **Example Recipes**:
    - **Hello Derp**: A simple C-based "Hello World" application (`recipes-derp`).
    - **Example Recipe**: Demonstrates build-time banners (`recipes-example`).

## Usage

This project uses [kas](https://github.com/siemens/kas) for configuration and build management.

### Building
To build the default QEMU image:
```bash
kas build qemu.yaml
```

To build for Raspberry Pi:
```bash
kas build raspberry3.yaml
# or
kas build raspberry4.yaml
```

To build for i.MX93 EVK:
```bash
kas build imx93evk.yaml
```

### entering Shell
To enter the BitBake environment shell:
```bash
kas shell qemu.yaml
```

### Running QEMU
Boot the built image in QEMU:
```bash
runqemu qemux86-64 nographic slirp
```

**Exiting QEMU:**
Press `Ctrl+a` then `c` to enter the QEMU monitor, then type `quit`.

## References
- See `.kas_shell_history` for the learning path and command history.