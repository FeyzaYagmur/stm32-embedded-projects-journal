# Asynchronous I2C Communication: Non-Blocking Serialization via HAL_I2C_Master_Transmit_IT

This project implements non-blocking serial transmission on an STM32 microcontroller using HAL_I2C_Master_Transmit_IT. Instead of waiting in a blocking polling loop during bus transmission, outgoing payloads are serialized in the background via hardware interrupts. The transfer lifecycle is managed through HAL_I2C_MasterTxCpltCallback without halting the CPU superloop.

## Architecture & Module Breakdown

* motor_driver.h: Target hardware address definitions, command frame formats, and HAL_I2C_Master_Transmit_IT API wrappers.
* motor_driver.c: Driver layer encapsulating bus presence checks and interrupt-driven transmission execution.
* main.c: Application layer executing startup handshakes, dispatching asynchronous payloads, and asserting status outputs inside the completion callback.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, I2C1 Event/Error Interrupts Enabled in NVIC)
* Target Slave Address: 0x58 (Shifted to 8-bit format: 0xB0)
* Status Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Presence Verification: Asserts device availability via HAL_I2C_IsDeviceReady. If the device does not acknowledge, the CPU enters an error blink state.
2. Interrupt Transmission: Dispatches the command frame using HAL_I2C_Master_Transmit_IT. Control returns to the main thread immediately.
3. Background Processing: The I2C peripheral hardware shifts bytes across the bus using hardware interrupts, avoiding blocking delays.
4. Transfer Complete Callback: Upon physical bus release, HAL_I2C_MasterTxCpltCallback triggers automatically and asserts PA5 HIGH.
