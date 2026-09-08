# KFS 1

Kernel minimal bootable avec GRUB, affichant `42` à l'ecran.

## Compiler

```bash
make
```

La compilation utilise `-ffreestanding`, `-fno-builtin` et
`-fno-stack-protector`. Le noyau est lie directement avec `ld` et `-nostdlib`,
donc aucune bibliotheque Linux n'est ajoutee. Les options `-fno-exception` et
`-fno-rtti` concernent C++ ; ce projet est ecrit en C.

## Lancer le kernel

```bash
qemu-system-i386 -cdrom kfs.iso
```

Selectionner `kfs` dans le menu GRUB.

## Nettoyer

```bash
make clean
```
