# Modular Register-Level I2C Sensor Driver: Memory-Mapped Configuration & Telemetry Ingestion

This project implements a professional, layered embedded driver for memory-mapped I2C peripherals (e.g., accelerometers, IMUs, environmental sensors) on an STM32 microcontroller. By leveraging STM32 HAL memory addressing APIs (HAL_I2C_Mem_Write and HAL_I2C_Mem_Read), internal hardware registers are abstracted behind a clean C interface (`sensor.h` and `sensor.c`), eliminating raw register offset management in the main application flow.

## Architecture & Module Breakdown

* sensor.h: Target slave address definitions, memory register map macros, and public hardware driver prototypes.
* sensor.c: Low-level driver implementing bus presence handshaking, configuration register programming, and telemetry register reading.
* main.c: Application layer executing device presence verification, sensor awakening/initialization, and periodic threshold-triggered actuation.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, 8-bit memory addressing format)
* Target Sensor Address: 0x68 (Shifted to 8-bit format: 0xD0)
* Register Map:
  * REG_CONFIG (0x00): Sensor power management / configuration register.
  * REG_DATA_OUT (0x01): Telemetry output register.
* Actuator Output: User Status LED on PA5 (Push-Pull Output)

## Operational Execution Flow

1. Presence Verification: Checks physical bus presence via HAL_I2C_IsDeviceReady. If the device does not respond with an ACK, execution enters a 10 Hz blink fault loop.
2. Device Awakening: Programs the configuration register (REG_CONFIG) with 0x01 to activate measurement mode.
3. Telemetry Ingestion: Continuously polls REG_DATA_OUT every 200 ms. If the ingested metric exceeds the safety threshold (50), PA5 is driven HIGH; otherwise, PA5 is driven LOW.
