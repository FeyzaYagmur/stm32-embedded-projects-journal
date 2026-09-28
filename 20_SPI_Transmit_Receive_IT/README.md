# Asynchronous SPI Register Operations: Non-Blocking Chained Interrupt Architecture (IT)

This project demonstrates an asynchronous, interrupt-driven SPI driver on an STM32 microcontroller using HAL_SPI_Transmit_IT and HAL_SPI_Receive_IT. Because the SPI peripheral lacks a native memory-addressing hardware engine (unlike I2C), non-blocking register reads require a chained interrupt state machine: the MCU transmits the target register offset asynchronously, transfers bus control within HAL_SPI_TxCpltCallback without dropping the Chip Select (CS) line, and finalizes data ingestion within HAL_SPI_RxCpltCallback.

## Architecture & Module Breakdown

* spi_driver.h: Hardware CS GPIO pin mappings, register map offsets, and asynchronous IT function prototypes.
* spi_driver.c: Driver implementation handling static buffer retention, manual CS latch control, chained interrupt handoffs, and completion callbacks.
* main.c: Application layer orchestrating system clock initialization, asynchronous peripheral awakening, periodic non-blocking sampling, and actuator threshold control.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: SPI1 (Master Mode, Full-Duplex, 8-Bit Data Size, NVIC Interrupts Enabled)
* Bus Lines:
  * SCK: PA5
  * MISO: PA6
  * MOSI: PA7
  * Software CS (NSS): PA4 (GPIO Output, Push-Pull, Pull-Up)
* Target Peripheral Register Map:
  * REG_SHUTDOWN (0x0C): Device power/sleep management register.
  * REG_DATA_X (0x01): Telemetry output measurement register.
* Status Indicator: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Initialization: CS (PA4) is initialized HIGH. A non-blocking write to REG_SHUTDOWN brings the target peripheral out of sleep mode.
2. Chained Read Trigger: The application calls SPI_Driver_Read_Reg_IT, asserting CS to LOW and dispatching the register offset via HAL_SPI_Transmit_IT. The main thread immediately resumes execution.
3. Transmission Callback Handoff: When the register address finishes transmitting across the MOSI line, hardware invokes HAL_SPI_TxCpltCallback. Rather than pulling CS HIGH, the driver detects an active read request and arms HAL_SPI_Receive_IT.
4. Reception Completion: Once the incoming telemetry byte is clocked into the target buffer, HAL_SPI_RxCpltCallback drives CS HIGH to release the bus and clears the state pointer.
