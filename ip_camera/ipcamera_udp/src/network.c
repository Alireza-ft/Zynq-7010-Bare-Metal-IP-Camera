#include "network.h"

#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "netif/xadapter.h"

#include "platform.h"

struct netif server_netif;

static unsigned char mac_address[] = { 0x00, 0x0A, 0x35, 0x00, 0x1E, 0x53 };

static ip4_addr_t ipaddr;
static ip4_addr_t netmask;
static ip4_addr_t gateway;

#define EMAC_BASE XPAR_XEMACPS_0_BASEADDR

/* Read-only register dump. Never re-initialize the MAC from here. */
static void print_mac_status(const char *tag)
{
    xil_printf("[%s] NWCTRL=%08X NWCFG=%08X NWSR=%08X\r\n", tag,
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_NWCTRL_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_NWCFG_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_NWSR_OFFSET));
    xil_printf("[%s] TXSR=%08X RXSR=%08X TXQBASE=%08X RXQBASE=%08X\r\n", tag,
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_TXSR_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_RXSR_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_TXQBASE_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_RXQBASE_OFFSET));
    xil_printf("[%s] TX=%u RX=%u RX_FCS_ERR=%u RX_RES_ERR=%u\r\n", tag,
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_TXCNT_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_RXCNT_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_RXFCSCNT_OFFSET),
        XEmacPs_ReadReg(EMAC_BASE, XEMACPS_RXRESERRCNT_OFFSET));
}

int network_init(void)
{
    xil_printf("[NET] init start\r\n");

    platform_init();
    lwip_init();

    IP4_ADDR(&ipaddr, 192,168,1,38);
    IP4_ADDR(&netmask, 255,255,255,0);
    IP4_ADDR(&gateway, 192,168,1,1);

    if (!xemac_add(&server_netif, &ipaddr, &netmask, &gateway,
                   mac_address, EMAC_BASE))
    {
        xil_printf("[NET] xemac_add FAILED\r\n");
        return -1;
    }

    netif_set_default(&server_netif);
    platform_enable_interrupts();
    netif_set_up(&server_netif);

    xil_printf("[NET] link %s, IP %d.%d.%d.%d\r\n",
        netif_is_link_up(&server_netif) ? "UP" : "DOWN",
        ip4_addr1(&ipaddr), ip4_addr2(&ipaddr),
        ip4_addr3(&ipaddr), ip4_addr4(&ipaddr));

    print_mac_status("after init");
    return 0;
}

void network_poll(void)
{
    static unsigned cnt = 0;

    if (++cnt % 500000 == 0)
    {
        print_mac_status("poll");
    }

    xemacif_input(&server_netif);
}

void network_debug_dump(void)
{
    print_mac_status("dump");
}