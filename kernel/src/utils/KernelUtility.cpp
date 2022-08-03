#include "utils/KernelUtility.hpp"

#include "TextRenderer.hpp"
#include "interrupts/InterruptDescriptorTable.hpp"
#include "interrupts/Interrupts.hpp"
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

    InterruptDescriptor *interrupt_PageFault = (InterruptDescriptor *)(idtRegister.Offset + 0xe * sizeof(InterruptDescriptor));
    interrupt_PageFault->setOffset((uint64_t)handler_PageFault);
    interrupt_PageFault->TypesAttrs = IDT_TYPES_ATTRS_INTERRUPT_GATE;
    interrupt_PageFault->Selector = 0x08;

    asm("lidt %0"
        :
        : "m"(idtRegister));
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
