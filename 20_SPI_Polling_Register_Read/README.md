# Synchronous SPI Telemetry Ingestion: Register-Addressed Polling Read via Transmit & Receive

This project demonstrates register-level data acquisition from an SPI peripheral on an STM32 microcontroller using blocking HAL_SPI_Transmit and HAL_SPI_Receive transactions. To query internal telemetry without a native memory-addressing hardware engine, the MCU frames the interaction within a single Chip Select (CS) cycle: it first transmits the target register offset, keeps CS asserted (Active-LOW), clocks in the resulting telemetry byte over MISO, and safely latches the bus by de-asserting CS (Active-HIGH).

## Architecture & Module Breakdown

* spi_driver.h: GPIO mappings for software Chip Select (CS), peripheral register maps, and synchronous write/read API prototypes.
* spi_driver.c: Low-level driver implementation handling CS line assertion, register addressing, data clocking, and failsafe latch de-assertion.
* main.c: Application layer orchestrating hardware initialization, peripheral wake-up, periodic register polling, and status LED threshold driving.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: SPI1 (Master Mode, Full-Duplex, 8-Bit Data Size, CPOL = Low, CPHA = 1 Edge)
* Bus Lines:
  * SCK: PA5
  * MISO: PA6
  * MOSI: PA7
  * Software CS (NSS): PA4 (GPIO Output, Push-Pull, Pull-Up)
* Target Peripheral Register Map:
  * REG_SHUTDOWN (0x0C): Device power/sleep control register.
  * REG_DATA_X (0x01): Telemetry output measurement register.
* Actuator Output: User Status LED on PA5 (Active-High Output)

## Operational Execution Flow

1. Startup Bus State: CS (PA4) is explicitly initialized to HIGH to prevent bus contention.
2. Device Awakening: Transmits 0x01 to REG_SHUTDOWN (0x0C) to take the peripheral out of power-down mode.
3. Telemetry Ingestion Cycle:
   * Asserts CS to LOW.
   * Transmits the requested register address (REG_DATA_X).
   * Generates clock cycles to read the incoming data byte into SRAM via HAL_SPI_Receive.
   * De-asserts CS to HIGH via a deterministic single-exit pattern to maintain bus safety.
4. Threshold Evaluation: If the acquired telemetry byte exceeds 50, PA5 is driven HIGH; otherwise, PA5 is driven LOW.
