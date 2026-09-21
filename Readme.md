# Zynq-7010 Bare-Metal IP Camera

A bare-metal IP camera project based on the Xilinx Zynq-7010 SoC.

The project is designed to capture image data, store video frames in DDR memory using AXI VDMA, and transmit the captured frame data over Ethernet using UDP.

## Overview

This project is implemented as a bare-metal application running on the ARM Cortex-A9 processor of the Zynq-7010, without an operating system.

The main data path is:

Camera
   |
   v
Video / AXI Stream
   |
   v
AXI VDMA
   |
   v
DDR Frame Buffer
   |
   v
Zynq-7010 ARM
   |
   v
lwIP
   |
   v
UDP
   |
   v
Ethernet
   |
   v
Host Computer


## Main Technologies

- Xilinx Zynq-7010
- Bare-Metal C
- Vivado
- Vitis
- AXI VDMA
- AXI4-Stream
- DDR Memory
- Xilinx GEM Ethernet
- lwIP
- UDP
- Ethernet PHY
- MDIO
- Xilinx GIC


## Software Architecture

The current software is divided into several modules:

    main.c
    |
    +-- network.c / network.h
    |       |
    |       +-- Ethernet initialization
    |       +-- lwIP initialization
    |       +-- PHY status
    |       +-- Network polling
    |
    +-- udp.c / udp.h
    |       |
    |       +-- UDP initialization
    |       +-- UDP connection
    |       +-- UDP packet transmission
    |
    +-- vdma.c / vdma.h
    |       |
    |       +-- VDMA initialization
    |       +-- VDMA configuration
    |       +-- Frame buffer management
    |       +-- VDMA debugging
    |
    +-- frame_tx.c / frame_tx.h
            |
            +-- Frame buffer access
            +-- Frame splitting
            +-- UDP frame transmission


## Ethernet Configuration

The current development configuration uses a static IP setup:

    Board IP       : 192.168.1.38
    Netmask        : 255.255.255.0
    Gateway        : 192.168.1.1

    Destination IP : 192.168.1.70
    UDP Port       : 5001

These values are currently used for development and testing and may change during the project.


## Ethernet and UDP

The Ethernet subsystem uses the Zynq PS GEM Ethernet controller together with the lwIP network stack.

The current implementation includes:

- Ethernet initialization
- PHY initialization and status checking
- PHY auto-negotiation monitoring
- Link status checking
- lwIP initialization
- Network interface configuration
- UDP PCB creation
- UDP connection
- UDP packet transmission
- Ethernet register debugging
- PHY register debugging

A small UDP test packet is used during development to verify the Ethernet and UDP path before enabling full-frame transmission.


## AXI VDMA

AXI VDMA is used to receive the video stream and write image data directly into DDR memory.

The VDMA configuration includes:

- Frame width
- Frame height
- Bytes per pixel
- Frame stride
- Frame buffer address
- Circular buffering
- S2MM write channel

The software also provides debugging functions for checking VDMA status and inspecting the framebuffer contents.


## Frame Buffer

The captured frame is stored in DDR memory.

The ARM processor accesses the framebuffer after VDMA has written the image data.

Because VDMA and the ARM processor access the same memory, cache coherency must be handled explicitly.

Before reading the framebuffer from the CPU, the corresponding cache range is invalidated:

    Xil_DCacheInvalidateRange(...)

This ensures that the CPU does not read stale cached data instead of the data written by VDMA.


## UDP Frame Transmission

The complete frame is divided into multiple UDP packets before transmission.

The basic process is:

    DDR Frame Buffer
           |
           v
       Frame Data
           |
           v
    Split into packets
           |
           +---- UDP Packet 0
           |
           +---- UDP Packet 1
           |
           +---- UDP Packet 2
           |
           +---- ...
           |
           +---- UDP Packet N
           |
           v
        Ethernet
           |
           v
      Host Computer


The frame transmission logic is implemented in:

    frame_tx.c

The main frame transmission function is:

    udp_send_frame()

Individual UDP payloads are sent using:

    udp_send_packet()


## Debugging

The project contains extensive UART debug output for low-level debugging.

Current debugging information includes:

- Ethernet initialization
- PHY auto-negotiation
- PHY link status
- PHY registers
- GEM Ethernet registers
- Network interface flags
- TX/RX status
- Ethernet counters
- UDP return codes
- VDMA status
- Framebuffer contents
- Cache handling

Wireshark is also used during development to inspect network traffic, including:

- ARP packets
- Ethernet frames
- UDP packets
- Source and destination MAC addresses
- Source and destination IP addresses
- UDP ports


## Project Structure

    src/
    |
    +-- main.c
    |
    +-- network.c
    +-- network.h
    |
    +-- udp.c
    +-- udp.h
    |
    +-- vdma.c
    +-- vdma.h
    |
    +-- frame_tx.c
    +-- frame_tx.h
    |
    +-- platform.c
    +-- platform.h


## Development Environment

The project is developed using the Xilinx FPGA development flow:

- Vivado
- Vitis
- Zynq-7000 Processing System
- Standalone / Bare-Metal BSP
- lwIP
- Xilinx drivers


## Current Development Goals

The main goal of the project is to build a complete embedded IP camera capable of:

1. Capturing image data from a camera.
2. Receiving the video stream through the FPGA logic.
3. Transferring frames to DDR memory using AXI VDMA.
4. Accessing the captured frame from the ARM processor.
5. Splitting the frame into UDP packets.
6. Transmitting the packets over Ethernet.
7. Receiving and reconstructing the frame on a PC.
8. Eventually providing continuous real-time video streaming.


## Development Status

This project is still under active development.

The Ethernet, lwIP, UDP, VDMA, framebuffer, and debugging components are currently being developed and tested.

The main focus at the current stage is debugging and validating the Ethernet communication path, VDMA frame capture, framebuffer access, and UDP transmission before completing the full camera-to-network pipeline.

More features, optimizations, hardware integration, and debugging improvements will be added as development continues.


## Status

    🚧 UNDER DEVELOPMENT
