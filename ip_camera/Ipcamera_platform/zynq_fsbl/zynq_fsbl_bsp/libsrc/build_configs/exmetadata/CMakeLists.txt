
cmake_minimum_required(VERSION 3.15)
project(bsp)
        
set(DRIVER_TARGETS axivdma;coresightps_dcc;devcfg;dmaps;emacps;gpiops;scugic;scutimer;scuwdt;sdps;nandps;uartps;xadcps)
set(DRIVER_LOCATIONS E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/axivdma_v6_15;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/coresightps_dcc_v1_10;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/devcfg_v3_8;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/dmaps_v2_10;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/emacps_v3_20;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/gpiops_v3_13;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/scugic_v5_3;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/scutimer_v2_6;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/scuwdt_v2_5;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/sdps_v4_3;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/nandps_v2_9;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/uartps_v3_14;E:/xilinx/Vitis/2024.1/data/embeddedsw/XilinxProcessorIPLib/drivers/xadcps_v2_7)

list(LENGTH DRIVER_TARGETS no_of_drivers)
set(index 0)

while(${index} LESS ${no_of_drivers})
    list(GET DRIVER_TARGETS ${index} drv)
    list(GET DRIVER_LOCATIONS ${index} drv_dir)
    set(exp_dir "${drv_dir}/examples")
    add_custom_target(
        ${drv}_example ALL
        COMMAND lopper -f -O C:/Users/Arf/Desktop/IP-Camera/ip_camera/ipcamera_platform/zynq_fsbl/zynq_fsbl_bsp/libsrc/${drv} C:/Users/Arf/Desktop/IP-Camera/ip_camera/ipcamera_platform/zynq_fsbl/zynq_fsbl_bsp/hw_artifacts/ps7_cortexa9_0_baremetal.dts -- bmcmake_metadata_xlnx ps7_cortexa9_0 ${exp_dir} drvcmake_metadata
        BYPRODUCTS x${drv}_exlist.yaml)
    MATH(EXPR index "${index}+1")
endwhile()
