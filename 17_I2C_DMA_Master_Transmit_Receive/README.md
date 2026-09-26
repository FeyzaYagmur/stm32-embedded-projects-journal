# Hardware-Accelerated I2C Bus Pipeline: Master Transmit & Receive via DMA Channels

This project implements fully hardware-delegated I2C bidirectional communication on an STM32 microcontroller using Direct Memory Access (DMA). By configuring HAL_I2C_Master_Transmit_DMA for payload transmission and HAL_I2C_Master_Receive_DMA for telemetry acquisition, serial data transfers occur directly between the I2C peripheral registers and SRAM without CPU byte-by-byte interrupt handling. The transaction lifecycles are monitored via HAL_I2C_MasterTxCpltCallback and HAL_I2C_MasterRxCpltCallback.

## Architecture & Module Breakdown

* io_expander.h: Hardware address definitions, buffer size declarations, and DMA API prototypes.
* io_expander.c: Hardware abstraction driver managing bus readiness handshaking and DMA transfer arming.
* main.c: Application layer orchestrating startup device checks, initialization transmit dispatch, periodic telemetry sampling, and callback-driven actuator control.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, RX/TX DMA Streams Enabled)
* Target Peripheral Address: 0x20 (Shifted to 8-bit format: 0x40)
* Status Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Presence Verification: Checks hardware presence using HAL_I2C_IsDeviceReady. If the slave fails to acknowledge, the system halts inside a 10 Hz fault blink loop.
2. Startup DMA Transmission: Arms HAL_I2C_Master_Transmit_DMA with an initialization byte (0xAA). Upon transfer completion, HAL_I2C_MasterTxCpltCallback triggers and asserts PA5.
3. Periodic DMA Telemetry Ingestion: Continuously dispatches non-blocking read requests using HAL_I2C_Master_Receive_DMA every 250 ms.
4. Asynchronous Evaluation: When incoming data is loaded into memory, HAL_I2C_MasterRxCpltCallback executes, evaluating whether the ingested metric exceeds the safety threshold (50) to actuate PA5.
