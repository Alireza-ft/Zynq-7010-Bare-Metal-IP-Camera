#ifndef VDMA_H
#define VDMA_H

#include "xstatus.h"

#define FRAME_WIDTH         320
#define FRAME_HEIGHT        240
#define BYTES_PER_PIXEL     2

#define FRAME_STRIDE        (FRAME_WIDTH * BYTES_PER_PIXEL)
#define FRAME_BUFFER_ADDR   0x10000000

int vdma_init(void);
int vdma_start(void);
void vdma_dump_framebuffer(void);
void vdma_debug_status(void);

#endif