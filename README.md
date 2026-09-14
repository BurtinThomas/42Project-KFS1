# KFS 1

Minimal bootable kernel using GRUB and displaying `42` on screen.

## Build

```bash
make
```

The build uses `-ffreestanding`, `-fno-builtin`, and
`-fno-stack-protector`. The kernel is linked directly with `ld` and
`-nostdlib`, so no Linux libraries are included. The `-fno-exceptions` and
`-fno-rtti` options apply to C++; this project is written in C.

## Run the kernel

```bash
qemu-system-i386 -cdrom kfs.iso
```

Select `kfs` from the GRUB menu.

## Clean the build

```bash
make clean
```
