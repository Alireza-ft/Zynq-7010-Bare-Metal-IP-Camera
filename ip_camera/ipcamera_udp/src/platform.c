#include "platform.h"

#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

// This global is referenced by name by Xilinx's xemacpsif driver
// to hook up the Ethernet RX/TX interrupt. If the linker reports
// an undefined reference to a differently-named GIC instance,
// rename this variable to match.
XScuGic InterruptController;

void platform_init(void)
{
    XScuGic_Config *gic_cfg;
    int status;

    xil_printf("[DEBUG] platform_init: looking up GIC config\r\n");

    gic_cfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
    if (gic_cfg == NULL) {
        xil_printf("[DEBUG] platform_init: ERROR - GIC config not found\r\n");
        return;
    }

    status = XScuGic_CfgInitialize(&InterruptController, gic_cfg, gic_cfg->CpuBaseAddress);
    if (status != XST_SUCCESS) {
        xil_printf("[DEBUG] platform_init: ERROR - GIC CfgInitialize failed, status=%d\r\n", status);
        return;
    }

    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                  (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                  &InterruptController);

    xil_printf("[DEBUG] platform_init: GIC initialized OK\r\n");
}

void platform_enable_interrupts(void)
{
    Xil_ExceptionEnable();
    xil_printf("[DEBUG] platform_enable_interrupts: CPU interrupts enabled\r\n");
}

XScuGic *platform_get_gic_instance(void)
{
    return &InterruptController;
}