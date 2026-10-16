#include "xil_printf.h"
#include "network.h"
#include "udp.h"
#include "vdma.h"
#include "frame_tx.h"

#include <string.h>

int main(void)
{
    xil_printf("\r\n\r\n[DEBUG] ========== Project Start ==========\r\n");

    if (network_init() != 0) {
        xil_printf("[DEBUG] network_init FAILED, halting\r\n");
        return -1;
    }

    if (vdma_init() != 0) {
        xil_printf("[DEBUG] vdma_init FAILED, halting\r\n");
        return -1;
    }

    memset((void *)FRAME_BUFFER_ADDR, 0x00, FRAME_WIDTH * FRAME_HEIGHT * BYTES_PER_PIXEL);
    xil_printf("[DEBUG] Framebuffer memset done\r\n");

    if (vdma_start() != 0) {
        xil_printf("[DEBUG] vdma_start FAILED, halting\r\n");
        return -1;
    }

    xil_printf("[DEBUG] Waiting for VDMA to fill buffer...\r\n");
    for (volatile int i = 0; i < 50000000; i++) {}

    vdma_debug_status();
    vdma_dump_framebuffer();

    if (udp_socket_init() != 0) {
        xil_printf("[DEBUG] udp_socket_init FAILED, halting\r\n");
        return -1;
    }

    xil_printf("[DEBUG] Polling network before first send...\r\n");
    for (int i = 0; i < 1000; i++) {
        network_poll();
    }

    network_debug_dump();

    udp_send_test();

    network_debug_dump();

    // udp_send_frame();  // disabled until ethernet is confirmed working

    xil_printf("[DEBUG] Entering main poll loop\r\n");
    while (1)
    {
        network_poll();
    }

    return 0;
}




