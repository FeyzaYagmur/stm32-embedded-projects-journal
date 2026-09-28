# Zero-Copy SPI Peripheral Driver: Hardware-Accelerated Chained DMA Pipeline

This project implements a fully offloaded SPI driver on an STM32 microcontroller using Direct Memory Access (DMA) channels. Because SPI lacks native register-addressing hardware mechanisms, non-blocking telemetry acquisition is achieved through an autonomous, chained DMA architecture: the memory system serializes the register query via HAL_SPI_Transmit_DMA, transitions bus control within HAL_SPI_TxCpltCallback without dropping the Chip Select (CS) line, and finalizes direct-to-SRAM reception via HAL_SPI_Receive_DMA.

## Architecture & Module Breakdown

* spi_driver.h: Hardware CS GPIO pin mappings, register map offsets, and DMA function prototypes.
* spi_driver.c: Driver implementation managing static buffer persistence, manual CS assertions, DMA handoffs, and interrupt-safe completion callbacks.
* main.c: Application layer orchestrating hardware DMA initialization, device awakening, periodic DMA read triggers, and asynchronous threshold validation.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Communication Bus: SPI1 (Master Mode, Full-Duplex, 8-Bit Data Size, TX/RX DMA Streams Enabled)
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

1. Bus Initialization: CS (PA4) defaults to HIGH. A non-blocking DMA transmission writes 0x01 to REG_SHUTDOWN to awaken the target peripheral.
2. Chained DMA Request: Calling SPI_Driver_Read_Reg_DMA asserts CS to LOW and initiates DMA serialization of the register offset over MOSI. The CPU resumes immediately.
3. Chained Reception Transition: Once the register byte is shifted out, HAL_SPI_TxCpltCallback executes, keeping CS asserted and instantly arming HAL_SPI_Receive_DMA.
4. Autonomous Ingestion & Latch Release: The SPI RX DMA channel streams the incoming byte directly into SRAM. Upon completion, HAL_SPI_RxCpltCallback sets CS HIGH, resets the buffer reference, and performs threshold actuation on PA5.
