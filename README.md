# QEMU Jokto Project

This project is a learning platform for Yocto and Kas, demonstrating how to add custom layers, create recipes, and modify existing system components.

## meta-foo Layer

The `meta-foo` layer is a custom layer designed to demonstrate:
- **Layer Creation**: How to structure a custom Yocto layer.
- **Recipe Customization**: Using `bbappend` files to modify existing recipes (e.g., `base-files`).
- **File Overrides**: Replacing system files like `/etc/motd` with custom versions using proper directory precedence (e.g., `recipes-core/base-files/files/poky/`).

### Key Features
- **Custom MOTD**: Replaces the default system message of the day (motd) with a custom "Otter Side" message.
- **Example Recipes**: Includes placeholders and examples for learning purposes.

## Usage

This project uses [kas](https://github.com/siemens/kas) for configuration and build management.

### Building
To build the default QEMU image:
```bash
kas build kas/qemu.yaml
```

### entering Shell
To enter the BitBake environment shell:
```bash
kas shell kas/qemu.yaml
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