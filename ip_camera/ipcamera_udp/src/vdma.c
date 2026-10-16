#include "vdma.h"

#include "xparameters.h"
#include "xaxivdma.h"
#include "xil_printf.h"
#include "xil_cache.h"

#include <string.h>

static XAxiVdma Vdma;

int vdma_start(void)
{
    XAxiVdma_DmaSetup Setup;
    UINTPTR BufferAddr;
    int Status;

    memset(&Setup, 0, sizeof(Setup));

    Setup.VertSizeInput = FRAME_HEIGHT;
    Setup.HoriSizeInput = FRAME_WIDTH * BYTES_PER_PIXEL;
    Setup.Stride = FRAME_STRIDE;
    Setup.FrameDelay = 0;
    Setup.EnableCircularBuf = 1;
    Setup.EnableSync = 0;
    Setup.PointNum = 0;
    Setup.EnableFrameCounter = 0;
    Setup.FixedFrameStoreAddr = 0;

    Status = XAxiVdma_DmaConfig(&Vdma, XAXIVDMA_WRITE, &Setup);
    if (Status != XST_SUCCESS)
    {
        xil_printf("[DEBUG] VDMA Config Failed, Status=%d\r\n", Status);
        return XST_FAILURE;
    }
    xil_printf("[DEBUG] VDMA Config OK\r\n");

    BufferAddr = FRAME_BUFFER_ADDR;
    Status = XAxiVdma_DmaSetBufferAddr(&Vdma, XAXIVDMA_WRITE, &BufferAddr);
    if (Status != XST_SUCCESS)
    {
        xil_printf("[DEBUG] VDMA Buffer Failed, Status=%d\r\n", Status);
        return XST_FAILURE;
    }
    xil_printf("[DEBUG] VDMA Buffer Addr Set OK = 0x%08X\r\n", (unsigned int)BufferAddr);

    Status = XAxiVdma_DmaStart(&Vdma, XAXIVDMA_WRITE);
    if (Status != XST_SUCCESS)
    {
        xil_printf("[DEBUG] VDMA Start Failed, Status=%d\r\n", Status);
        return XST_FAILURE;
    }

    xil_printf("VDMA Started\r\n");
    return XST_SUCCESS;
}

void vdma_debug_status(void)
{
    u32 sr = XAxiVdma_ReadReg(Vdma.WriteChannel.ChanBase, XAXIVDMA_SR_OFFSET);

    xil_printf("[DEBUG] VDMA S2MM Status Reg = %08X\r\n", sr);
    xil_printf("[DEBUG]   Halted=%d Idle=%d Err_Internal=%d Err_Slave=%d Err_DecErr=%d FrameCntIrq=%d\r\n",
               sr & 0x1,
               (sr >> 1) & 0x1,
               (sr >> 4) & 0x1,
               (sr >> 5) & 0x1,
               (sr >> 6) & 0x1,
               (sr >> 12) & 0x1);

    int busy = XAxiVdma_IsBusy(&Vdma, XAXIVDMA_WRITE);
    xil_printf("[DEBUG] VDMA IsBusy (write channel) = %d\r\n", busy);
}

void vdma_dump_framebuffer(void)
{
    Xil_DCacheInvalidateRange(FRAME_BUFFER_ADDR, FRAME_WIDTH * FRAME_HEIGHT * BYTES_PER_PIXEL);
    xil_printf("[DEBUG] Cache invalidated before framebuffer dump\r\n");

    volatile unsigned int *ptr = (volatile unsigned int *)FRAME_BUFFER_ADDR;

    xil_printf("Frame Buffer:\r\n");
    for (int i = 0; i < 16; i++)
    {
        xil_printf("%08X\r\n", ptr[i]);
    }
}

int vdma_init(void)
{
    XAxiVdma_Config *Config;
    int Status;

    Config = XAxiVdma_LookupConfig(XPAR_XAXIVDMA_0_BASEADDR);
    if (Config == NULL)
    {
        xil_printf("[DEBUG] ERROR: VDMA config not found\r\n");
        return XST_FAILURE;
    }

    Status = XAxiVdma_CfgInitialize(&Vdma, Config, Config->BaseAddress);
    if (Status != XST_SUCCESS)
    {
        xil_printf("[DEBUG] ERROR: VDMA init failed, Status=%d\r\n", Status);
        return XST_FAILURE;
    }

    xil_printf("VDMA Initialized | BaseAddr=0x%08X\r\n", (unsigned int)Config->BaseAddress);
    return XST_SUCCESS;
}