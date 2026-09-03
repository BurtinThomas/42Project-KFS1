# Variables
CC=i386-elf-gcc
AS=i386-elf-as
LD=i386-elf-gcc
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-ffreestanding -O2 -nostdlib
ISO_DIR=isodir
BOOT_DIR=$(ISO_DIR)/boot
GRUB_DIR=$(BOOT_DIR)/grub

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
	$(AS) $(BOOT_SRC) -o $(BOOT_OBJ)

$(KERNEL_OBJ): $(KERNEL_SRC)
	$(CC) -c $(KERNEL_SRC) -o $(KERNEL_OBJ) $(CFLAGS)

$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) linker.ld
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(LDFLAGS) $(BOOT_OBJ) $(KERNEL_OBJ) -lgcc

$(ISO): $(KERNEL_BIN) $(GRUB_CFG)
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL_BIN) $(BOOT_DIR)/kfs.bin
	cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Nettoyage
clean:
	rm -f *.o *.bin $(ISO)
	rm -rf $(ISO_DIR)

re: clean all
.PHONY: all clean
