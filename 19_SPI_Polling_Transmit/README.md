# Synchronous SPI Peripheral Driver: Manual Chip Select & Register-Level Framing

This project implements a modular SPI peripheral driver on an STM32 microcontroller using blocking transmission via HAL_SPI_Transmit. Unlike I2C, SPI operates without device addressing bytes or hardware acknowledge (ACK) bits. Bus selection is asserted directly via software GPIO toggling of the Chip Select (CS/NSS) pin, while register manipulation is executed by framing data into a contiguous 2-byte payload: [Register Offset, Configuration Data].

## Architecture & Module Breakdown

* spi_driver.h: Hardware CS GPIO pin mappings, internal peripheral register maps, and synchronous write API prototypes.
* spi_driver.c: Driver implementation encapsulating CS assertion (Active-LOW), blocking SPI transmission, and CS de-assertion (Active-HIGH latching).
* main.c: Application layer orchestrating hardware peripheral initialization, device awakening, and parameter updates.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: SPI1 (Master Mode, Full-Duplex, 8-Bit Data Size, CPOL = Low, CPHA = 1 Edge)
* Bus Lines:
  * SCK: PA5
  * MOSI: PA7
  * MISO: PA6
  * Software CS (NSS): PA4 (GPIO Output, Push-Pull, Pull-Up)
* Target Register Map:
  * REG_SHUTDOWN (0x0C): Device power/sleep control register.
  * REG_DATA_X (0x01): Peripheral data/gain configuration register.

## Operational Execution Flow

1. CS Assertion (Active-LOW): PA4 is driven LOW to select the target peripheral and arm its internal shift register.
2. Frame Serialization: A 2-byte payload consisting of the target register address (e.g., REG_SHUTDOWN) and configuration parameter (0x01) is transmitted over MOSI using HAL_SPI_Transmit.
3. CS De-assertion (Active-HIGH): PA4 is driven HIGH immediately following transmission to latch the received byte into the internal device register and release the SPI bus.
