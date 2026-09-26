# Hardware-Accelerated Memory Bus Pipeline: Register Access via HAL_I2C_Mem_Write_DMA and Mem_Read_DMA

This project implements non-blocking, register-addressed I2C data transfers on an STM32 microcontroller using Direct Memory Access (DMA). Utilizing HAL_I2C_Mem_Write_DMA and HAL_I2C_Mem_Read_DMA, memory-mapped operations (targeting non-volatile EEPROMs or smart sensors) execute autonomously between internal registers and SRAM. Physical bus handshakes and buffer serialization are offloaded from the CPU, with event signaling managed via HAL_I2C_MemTxCpltCallback and HAL_I2C_MemRxCpltCallback.

## Architecture & Module Breakdown

* eeprom.h: Device addressing macros, memory register offsets, and public DMA memory-access API prototypes.
* eeprom.c: Hardware driver layer wrapping bus readiness verification and DMA memory transaction pipelines.
* main.c: Application layer executing device presence handshaking, startup calibration write, periodic register polling, and status LED threshold driving.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, 8-bit memory addressing format, RX/TX DMA Streams Enabled)
* Target EEPROM Address: 0x50 (Shifted to 8-bit format: 0xA0)
* Target Memory Register: REG_DATA_CELL (0x08)
* Actuator / Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Device Presence Verification: Asserts device responsiveness using HAL_I2C_IsDeviceReady. If no ACK is returned, the CPU halts within a visual fault blink trap (10 Hz).
2. Initialization Memory Write: Dispatches an initial data value (75) to target register offset 0x08 via HAL_I2C_Mem_Write_DMA without stalling the CPU core.
3. Periodic Telemetry Ingestion: Continuously arms HAL_I2C_Mem_Read_DMA every 250 ms to ingest the contents of register 0x08 directly into SRAM.
4. Asynchronous Threshold Validation: Upon DMA transfer completion, hardware triggers HAL_I2C_MemRxCpltCallback. If the ingested value exceeds 50, PA5 is driven HIGH; otherwise, PA5 is driven LOW.
