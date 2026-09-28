# Event-Driven SPI Telemetry Acquisition: Chained Non-Blocking Interrupts (IT)

This project demonstrates an asynchronous, event-driven SPI master interface on an STM32 microcontroller using HAL_SPI_Transmit_IT and HAL_SPI_Receive_IT. Because the SPI peripheral lacks a native memory-offset engine, non-blocking telemetry ingestion is coordinated via a chained interrupt state machine. The MCU transmits the target register offset, maintains the Chip Select (CS) line asserted across HAL_SPI_TxCpltCallback, and completes ingestion inside HAL_SPI_RxCpltCallback where data-dependent actuator control is immediately serviced.

## Architecture & Module Breakdown

* spi_driver.h: Hardware CS GPIO pin mappings, peripheral register definitions, and public asynchronous IT API prototypes.
* spi_driver.c: Low-level driver managing persistent static transfer buffers, CS line transitions, and chained interrupt handoffs.
* main.c: Application layer managing peripheral clocks, startup device awakening, periodic read triggers, and ISR callback threshold evaluation.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: SPI1 (Master Mode, Full-Duplex, 8-Bit Data Size, NVIC Global Interrupt Enabled)
* Pin Assignments:
  * SCK: PA5
  * MISO: PA6
  * MOSI: PA7
  * Software CS (NSS): PA4 (GPIO Output, Push-Pull, Pull-Up)
* Peripheral Register Map:
  * REG_SHUTDOWN (0x0C): Device power management register.
  * REG_DATA_X (0x01): Telemetry output measurement register.
* Actuator Output: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Initialization: CS (PA4) defaults to HIGH. A non-blocking interrupt write to REG_SHUTDOWN awakes the external peripheral from low-power mode.
2. Triggering Chained Acquisition: The superloop invokes SPI_Driver_Read_Reg_IT every 250 ms. The driver asserts CS to LOW and initiates background transmission of the register query byte via HAL_SPI_Transmit_IT.
3. Transmission Callback Handoff: When the address byte has fully clocked out over MOSI, hardware enters HAL_SPI_TxCpltCallback. Because a read pointer is pending, CS remains held LOW and HAL_SPI_Receive_IT is immediately armed.
4. Reception ISR & Threshold Servicing: Upon ingesting the telemetry byte into SRAM, HAL_SPI_RxCpltCallback de-asserts CS to HIGH, resets the buffer pointer, and evaluates the received value: if it exceeds 50, PA5 is driven HIGH; otherwise, PA5 is driven LOW.
