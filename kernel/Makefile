
OSNAME = NucleaOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = LinkScript.ld
ASM = nasm
CC = gcc
LD = ld

ASMFLAGS =
CFLAGS = -ffreestanding -fshort-wchar -fno-exceptions -mno-red-zone -I $(INCDIR) -g 
LDFLAGS = -T $(LDS) -static -shared -Bsymbolic -nostdlib -fno-stack-protection

BUILDDIR = bin
INCDIR = inc
OBJDIR = lib
SRCDIR = src

BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)         
SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.cpp_o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.asm_o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 1440 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/Uni2-Terminus16.psf ::

kernel: $(OBJS) link

$(OBJDIR)/interrupts/Interrupts.cpp_o: $(SRCDIR)/interrupts/Interrupts.cpp
	@echo \| ===== Compiling $^ ===== \|
	@mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -I $(INCDIR) -c $^ -o $@

$(OBJDIR)/%.cpp_o: $(SRCDIR)/%.cpp
	@echo \| ===== Compiling $^ ===== \|
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%.asm_o: $(SRCDIR)/%.asm
	@echo \| ===== Assembling $^ ===== \|
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@echo \| ===== Linking ===== \|
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

clean-lib:
	rm -rf $(OBJDIR)/*
