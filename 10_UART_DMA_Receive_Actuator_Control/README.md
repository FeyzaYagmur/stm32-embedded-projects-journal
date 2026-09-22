# UART DMA Receive Architecture: Non-Blocking Ingestion & Deterministic Actuator Control

This project demonstrates hardware-delegated serial reception using Direct Memory Access (DMA) on an STM32 microcontroller. By initiating HAL_UART_Receive_DMA for command byte streams, incoming serial data bypasses the CPU core entirely and is loaded directly into SRAM. The firmware achieves deterministic latency for bidirectional motor/actuator directional outputs without software polling overhead.

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Serial Interface: USART2 (115200 Baud, 8N1, RX DMA Stream Enabled)
* Motor Direction Output 1: PA0 (Digital Output, Push-Pull)
* Motor Direction Output 2: PA1 (Digital Output, Push-Pull)

## Key Engineering Principles

* DMA Receive Architecture: HAL_UART_Receive_DMA routes incoming serial bytes directly to memory, eliminating CPU core polling and software interrupt trapping during byte reception.
* Deterministic State Execution: Upon transfer completion, HAL_UART_RxCpltCallback evaluates the opcode instantly, minimizing actuator response latency.
* Continuous Re-Arming Pattern: The DMA reception stream is re-initialized at the base of the callback to ensure immediate readiness for subsequent actuation commands.
* Failsafe Directional Interlock: Prevents shoot-through conditions across H-bridge driver inputs by explicitly clearing opposite control lines on transitions.

## Command Set & Hardware Actions

* 'F' / 'f' (Forward): Sets PA0 HIGH and PA1 LOW.
* 'B' / 'b' (Backward): Sets PA0 LOW and PA1 HIGH.
* 'S' / 's' (Stop / Neutral): Clears both PA0 and PA1 to LOW.
