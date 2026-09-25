# Asynchronous Register Ingestion: Non-Blocking Telemetry Acquisition via HAL_I2C_Mem_Read_IT

This project demonstrates non-blocking telemetry acquisition from an I2C-interfaced ultrasonic distance sensor (HC-SR04 I2C adapter) on an STM32 microcontroller. By offloading internal register addressing and byte reception to hardware interrupts via HAL_I2C_Mem_Read_IT, the MCU ingests proximity data without CPU busy-wait loops. Once bytes are clocked and transferred into memory, hardware invokes HAL_I2C_MemRxCpltCallback to process threshold logic.

## Architecture & Module Breakdown

* hcsr04_i2c.h: Sensor address macros, internal register map definitions, and non-blocking memory read prototypes.
* hcsr04_i2c.c: Low-level driver encapsulating presence handshaking and interrupt-driven register read requests.
* main.c: Application layer orchestrating periodic read triggering, device verification, and threshold actuation inside the completion callback.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: I2C1 (Standard-mode 100 kHz, 8-bit memory addressing format, NVIC Interrupts Enabled)
* Sensor Bus Address: 0x57 (Shifted to 8-bit format: 0xAE)
* Target Register: REG_DISTANCE_DATA (0x01)
* Actuator Output: Proximity Alert LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Presence Verification: Checks hardware bus responsiveness using HAL_I2C_IsDeviceReady. If the sensor is unresponsive, execution enters a 10 Hz fault blink loop.
2. Asynchronous Read Trigger: The application issues a periodic non-blocking register read request via HAL_I2C_Mem_Read_IT every 200 ms. The CPU resumes immediately.
3. Hardware Serialization: The I2C peripheral transmits the register pointer (0x01) and clocks in the incoming distance byte in the background.
4. Completion Callback: Upon bus release, hardware triggers HAL_I2C_MemRxCpltCallback. If the measured proximity is under 20 cm, PA5 is driven HIGH; otherwise, PA5 is driven LOW.
