x86_64_ASM_SOURCES := $(shell find src/arch/x86_64 -name *.asm)
x86_64_ASM_OBJECTS := $(patsubst src/arch/x86_64/%.asm, build/x86_64/%.o, $(x86_64_ASM_SOURCES))

$(x86_64_ASM_OBJECTS): build/x86_64/%.o : src/arch/x86_64/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, src/arch/x86_64/%.asm, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(x86_64_ASM_OBJECTS)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/LinkerScript.ld $(x86_64_ASM_OBJECTS) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/NucleaOS.iso targets/x86_64/iso

run-x86_64: build-x86_64
	qemu-system-x86_64 -cdrom dist/x86_64/NucleaOS.iso