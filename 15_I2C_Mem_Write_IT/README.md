# Asynchronous Register Configuration: Non-Blocking Memory Writes via HAL_I2C_Mem_Write_IT

This project implements non-blocking register-level configuration on an STM32 microcontroller using HAL_I2C_Mem_Write_IT. By targeting internal hardware registers asynchronously, payload frames are transmitted across the I2C bus via hardware interrupts without stalling the main execution thread. Transmission finalization is handled through the dedicated HAL_I2C_MemTxCpltCallback ISR hook.

## Architecture & Module Breakdown

* stepper.h: Slave address macros, internal register address offsets, and asynchronous memory-write API prototypes.
* stepper.c: Low-level driver encapsulating bus presence checks and interrupt-driven register write transactions.
* main.c: Application layer executing startup presence verification, non-blocking configuration dispatch, and visual completion signaling.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, 8-bit memory addressing format, NVIC Interrupts Enabled)
* Stepper Driver Address: 0x60 (Shifted to 8-bit format: 0xC0)
* Target Register: REG_STEP_COUNT (0x01)
* Status Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Presence Handshake: Asserts stepper driver availability using HAL_I2C_IsDeviceReady. If unresponsive, the system halts inside a 10 Hz fault blink trap.
2. Asynchronous Memory Write: Dispatches the target step configuration (200 steps) to REG_STEP_COUNT via HAL_I2C_Mem_Write_IT. The CPU returns to the main loop immediately.
3. Interrupt Serialization: Hardware interrupts shift the register offset and parameter payload across the physical bus lines in the background.
4. Transmission Completion Hook: Once the final ACK is received and the bus line is released, HAL_I2C_MemTxCpltCallback fires and asserts PA5 HIGH.
