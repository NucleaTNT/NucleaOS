#include "utils/KernelUtility.hpp"

#include "interrupts/InterruptDescriptorTable.hpp"
#include "interrupts/Interrupts.hpp"
#include "interrupts/PIC.hpp"
#include "io/IOBus.hpp"
#include "io/graphics/TextRenderer.hpp"
#include "io/mouse/PS2_Mouse.hpp"
#include "memory/GlobalDescriptorTable.hpp"

void prepareMemory(BootInfo *bootInfo) {
    g_PageFrameAllocator.readEFIMemoryMap(bootInfo->MemMapInfo);
    size_t totalMemSize = getTotalMemorySize(bootInfo->MemMapInfo);

    /* Reserve pages of kernel just in case */
    size_t kernelSize = ((&_KernelEnd - &_KernelBegin) / PAGE_SIZE) + 1;
    g_PageFrameAllocator.lockPages((void *)&_KernelBegin, kernelSize);

    PageTable *pageMapLevel4 = (PageTable *)g_PageFrameAllocator.requestPage();
    memset(pageMapLevel4, 0, PAGE_SIZE);

    PageTableManager pageTableManager(pageMapLevel4);
    for (size_t i = 0; i < totalMemSize; i += PAGE_SIZE) {
        pageTableManager.mapMemory((void *)i, (void *)i);
    }

    uint64_t frameBuffBase = (uint64_t)bootInfo->GlobalFrameBuffer->BaseAddress;
    uint64_t frameBuffSize = (uint64_t)bootInfo->GlobalFrameBuffer->BufferSize + PAGE_SIZE;
    g_PageFrameAllocator.lockPages((void *)frameBuffBase, frameBuffSize / PAGE_SIZE + 1);
    for (size_t i = frameBuffBase; i < frameBuffBase + frameBuffSize; i += PAGE_SIZE) {
        pageTableManager.mapMemory((void *)i, (void *)i);
    }

    asm("mov %0, %%cr3"
        :
        : "r"(pageMapLevel4));

    memset(bootInfo->GlobalFrameBuffer->BaseAddress, 0, bootInfo->GlobalFrameBuffer->BufferSize);
}

IDTRegister idtRegister;
void registerInterrupt(void *handlerMethod, uint8_t entryOffset, uint8_t typesAttrs, uint8_t selector) {
    InterruptDescriptor *interruptDescriptor = (InterruptDescriptor *)(idtRegister.Offset + entryOffset * sizeof(InterruptDescriptor));
    interruptDescriptor->setOffset((uint64_t)handlerMethod);
    interruptDescriptor->TypesAttrs = typesAttrs;
    interruptDescriptor->Selector = selector;
}

void prepareInterrupts() {
    idtRegister.Size = 0x0fff;
    idtRegister.Offset = (uint64_t)g_PageFrameAllocator.requestPage();

    registerInterrupt((void *)handler_DoubleFault, 0x8, IDT_TYPES_ATTRS_INTERRUPT_GATE, 0x08);
    registerInterrupt((void *)handler_GeneralProtectionFault, 0xd, IDT_TYPES_ATTRS_INTERRUPT_GATE, 0x08);
    registerInterrupt((void *)handler_PageFault, 0xe, IDT_TYPES_ATTRS_INTERRUPT_GATE, 0x08);
    registerInterrupt((void *)handler_KeyboardInterrupt, 0x21, IDT_TYPES_ATTRS_INTERRUPT_GATE, 0x08);
    registerInterrupt((void *)handler_MouseInterrupt, 0x2c, IDT_TYPES_ATTRS_INTERRUPT_GATE, 0x08);

    /* Disable interrupts while loading new IDT. */
    asm("cli");

    asm("lidt %0"
        :
        : "m"(idtRegister));

    remapPIC();
}

static TextRenderer OUTPUT((FrameBuffer){}, (PSF1_Font){}, 0);
void initializeKernel(BootInfo *bootInfo) {
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GlobalDescriptorTable) - 1;
    gdtDescriptor.Offset = (uint64_t)&g_GlobalDescriptorTable;
    _loadGDT(&gdtDescriptor);

    prepareMemory(bootInfo);

    OUTPUT = TextRenderer(*bootInfo->GlobalFrameBuffer, *bootInfo->LoadedFont, 0xffffffff);
    g_TextRenderer = OUTPUT;

    prepareInterrupts();

    initPS2Mouse();

    /* Unmask PIC interrupts. */
    outb(PIC_MASTER_DATA, 0b11111001);
    outb(PIC_SLAVE_DATA, 0b11101111);

    /* Re-enable interrupts. */
    asm("sti");
}
