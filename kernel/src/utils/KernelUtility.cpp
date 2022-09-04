#include "utils/KernelUtility.hpp"

#include "interrupts/InterruptDescriptorTable.hpp"
#include "interrupts/Interrupts.hpp"
#include "interrupts/PIC.hpp"
#include "io/IOBus.hpp"
#include "io/graphics/TextRenderer.hpp"
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
void prepareInterrupts() {
    idtRegister.Size = 0x0fff;
    idtRegister.Offset = (uint64_t)g_PageFrameAllocator.requestPage();

    InterruptDescriptor *interrupt_DoubleFault = (InterruptDescriptor *)(idtRegister.Offset + 0x8 * sizeof(InterruptDescriptor));
    interrupt_DoubleFault->setOffset((uint64_t)handler_DoubleFault);
    interrupt_DoubleFault->TypesAttrs = IDT_TYPES_ATTRS_INTERRUPT_GATE;
    interrupt_DoubleFault->Selector = 0x08;

    InterruptDescriptor *interrupt_GeneralProtectionFault = (InterruptDescriptor *)(idtRegister.Offset + 0xd * sizeof(InterruptDescriptor));
    interrupt_GeneralProtectionFault->setOffset((uint64_t)handler_GeneralProtectionFault);
    interrupt_GeneralProtectionFault->TypesAttrs = IDT_TYPES_ATTRS_INTERRUPT_GATE;
    interrupt_GeneralProtectionFault->Selector = 0x08;

    InterruptDescriptor *interrupt_PageFault = (InterruptDescriptor *)(idtRegister.Offset + 0xe * sizeof(InterruptDescriptor));
    interrupt_PageFault->setOffset((uint64_t)handler_PageFault);
    interrupt_PageFault->TypesAttrs = IDT_TYPES_ATTRS_INTERRUPT_GATE;
    interrupt_PageFault->Selector = 0x08;

    InterruptDescriptor *interrupt_Keyboard = (InterruptDescriptor *)(idtRegister.Offset + 0x21 * sizeof(InterruptDescriptor));
    interrupt_Keyboard->setOffset((uint64_t)handler_KeyboardInterrupt);
    interrupt_Keyboard->TypesAttrs = IDT_TYPES_ATTRS_INTERRUPT_GATE;
    interrupt_Keyboard->Selector = 0x08;

    asm("cli");

    asm("lidt %0"
        :
        : "m"(idtRegister));

    remapPIC();

    /* Unmask keyboard interrupt. */
    outb(PIC_MASTER_DATA, 0b11111101);
    outb(PIC_SLAVE_DATA, 0b11111111);

    /* Re-enable interrupts. */
    asm("sti");
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
}
