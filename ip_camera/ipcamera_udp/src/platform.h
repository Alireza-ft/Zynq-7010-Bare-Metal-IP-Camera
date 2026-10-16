#ifndef PLATFORM_H
#define PLATFORM_H

#include "xscugic.h"

void platform_init(void);
void platform_enable_interrupts(void);
XScuGic *platform_get_gic_instance(void);

#endif