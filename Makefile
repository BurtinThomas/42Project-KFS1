CC := gcc
AS := as
LD := ld
GRUB := grub2-mkrescue

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra \
	-fno-builtin -fno-stack-protector -m32 -fno-pie -nodefaultlibs
LDFLAGS := -m elf_i386 -nostdlib
ASFLAGS := --32

ISO := kfs.iso
KERNEL := kfs.bin
ISO_ROOT := isodir
GRUB_ROOT := $(ISO_ROOT)/boot/grub

OBJECTS := boot.o kernel.o

.PHONY: all clean re

all: $(ISO)

boot.o: boot.s
	$(AS) $(ASFLAGS) $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ $(OBJECTS)

$(ISO): $(KERNEL) grub.cfg
	mkdir -p $(GRUB_ROOT)
	cp $(KERNEL) $(ISO_ROOT)/boot/kfs.bin
	cp grub.cfg $(GRUB_ROOT)/grub.cfg
	$(GRUB) -o $@ $(ISO_ROOT)
	rm -rf $(ISO_ROOT)

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	rm -rf $(ISO_ROOT)

re: clean all
