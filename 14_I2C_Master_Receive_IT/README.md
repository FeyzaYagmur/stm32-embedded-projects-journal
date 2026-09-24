# Asynchronous I2C Ingestion: Non-Blocking Telemetry Acquisition via HAL_I2C_Master_Receive_IT

This project implements non-blocking I2C data acquisition on an STM32 microcontroller using HAL_I2C_Master_Receive_IT. Instead of stalling the CPU in a blocking wait loop while clocking bits from the bus, the reception pipeline is delegated entirely to hardware interrupts. Incoming telemetry frames are captured asynchronously through HAL_I2C_MasterRxCpltCallback.

## Architecture & Module Breakdown

* ntc_sensor.h: Slave address macros, frame size parameters, and asynchronous reception API prototypes.
* ntc_sensor.c: Low-level driver layer encapsulating bus presence verification and interrupt-driven ingestion triggers.
* main.c: Application layer orchestrating periodic asynchronous read requests, fault traps, and threshold evaluation inside the completion callback.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, I2C1 Event/Error Interrupts Enabled in NVIC)
* Target Sensor Address: 0x48 (Shifted to 8-bit format: 0x90)
* Status Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Presence Verification: Checks hardware responsiveness via HAL_I2C_IsDeviceReady. If the sensor fails to acknowledge, the CPU enters a 10 Hz fault blink trap.
2. Asynchronous Ingestion Request: The application issues a periodic non-blocking read command via HAL_I2C_Master_Receive_IT every 500 ms. The CPU resumes immediately.
3. Hardware Serialization: The I2C peripheral clocks the bus lines and captures incoming data bytes in the background using NVIC interrupts.
4. Asynchronous Completion Hook: Upon bus cycle termination, hardware triggers HAL_I2C_MasterRxCpltCallback. The ingested metric is evaluated against a threshold (50) to actuate PA5.
