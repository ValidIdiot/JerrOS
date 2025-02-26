#include <jerrio.h>

#define QEMU_SHUTDOWN_PORT 0x604
#define BOCHS_SHUTDOWN_PORT 0xB004

void shutdown() {
    __asm__ __volatile__("outw %0, %1" : : "a"((uint16_t)0x2000), "Nd"(BOCHS_SHUTDOWN_PORT));

    __asm__ __volatile__("outw %0, %1" : : "a"((uint16_t)0x2000), "Nd"(QEMU_SHUTDOWN_PORT));

    while (1) {
        __asm__ __volatile__("hlt");
    }
}