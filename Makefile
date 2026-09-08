# Variables
TOOLCHAIN ?= native

ifeq ($(TOOLCHAIN),native)
CC=gcc
AS=as
LD=ld
ASFLAGS=--32
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-stack-protector -m32 -fno-pie
LDFLAGS=-m elf_i386 -nostdlib
LIBS=
else
CC=i386-elf-gcc
AS=i386-elf-as
LD=i386-elf-gcc
ASFLAGS=
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-stack-protector
LDFLAGS=-ffreestanding -O2 -nostdlib
LIBS=-lgcc
endif
ISO_DIR=isodir
BOOT_DIR=$(ISO_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub
GRUB_MKRESCUE ?= $(shell command -v grub-mkrescue 2>/dev/null || command -v grub2-mkrescue 2>/dev/null)
GRUB_COMPRESS ?= xz

# Fichiers
KERNEL_SRC=kernel.c
BOOT_SRC=boot.s
KERNEL_OBJ=kernel.o
BOOT_OBJ=boot.o
KERNEL_BIN=kfs.bin
ISO=kfs.iso
GRUB_CFG=grub.cfg

# Règle par défaut
all: $(ISO)

# Compilation
$(BOOT_OBJ): $(BOOT_SRC)
	$(AS) $(ASFLAGS) $(BOOT_SRC) -o $(BOOT_OBJ)

$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) -c $(KERNEL_SRC) -o $(KERNEL_OBJ) $(CFLAGS)

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(LDFLAGS) $(BOOT_OBJ) $(KERNEL_OBJ) $(LIBS)

$(ISO): $(KERNEL_BIN) $(GRUB_CFG)
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL_BIN) $(BOOT_DIR)/kfs.bin
	cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	$(GRUB_MKRESCUE) --compress=$(GRUB_COMPRESS) -o $(ISO) $(ISO_DIR)
	rm -rf $(ISO_DIR)

# Nettoyage
clean:
	rm -f *.o *.bin $(ISO)
	rm -rf $(ISO_DIR)

re: clean all
.PHONY: all clean
