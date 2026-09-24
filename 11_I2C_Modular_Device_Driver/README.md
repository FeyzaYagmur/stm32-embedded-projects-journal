# Modular I2C Peripheral Driver: Device Readiness, Polling Transmit & Telemetry Ingestion

This project demonstrates a modular, layered embedded software architecture for standard I2C bus transactions on an STM32 microcontroller. Hardware-level bus operations are abstracted into a dedicated driver library (`i2c_cihaz.h` and `i2c_cihaz.c`), decoupling transaction logic from the application superloop (`main.c`).

## Architecture & File Structure

* i2c_cihaz.h: Hardware abstraction definitions, 7-bit shifted slave addressing, and public driver APIs.
* i2c_cihaz.c: Implementation of I2C bus readiness checks, single-byte writes, and single-byte reads via STM32 HAL.
* main.c: Application layer implementing startup device verification, initial bus configuration, and a periodic telemetry processing loop.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, Open-Drain with external pull-ups)
* Slave Bus Address: 0x68 (Shifted to 8-bit format: 0xD0)
* Actuator Output: User Status LED on PA5 (Push-Pull Output)

## Operational Execution Flow

1. Startup Handshake: The MCU polls the I2C bus via HAL_I2C_IsDeviceReady. If the target device fails to acknowledge, execution traps into a high-frequency LED blink fault loop.
2. Initialization Write: Upon successful bus handshake, the MCU transmits an initialization byte (0xAA) to configure the target device.
3. Periodic Polling Loop: The application continuously queries the target device every 200 ms. If the read byte exceeds threshold (50), PA5 is driven HIGH; otherwise, PA5 is driven LOW.
