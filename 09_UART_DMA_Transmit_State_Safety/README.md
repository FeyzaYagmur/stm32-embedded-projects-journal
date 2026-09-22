# Non-Blocking UART Telemetry via DMA & Asynchronous Transfer Synchronization

This project demonstrates hardware-accelerated serial transmission using Direct Memory Access (DMA) on an STM32 microcontroller. The implementation features an atomic busy-flag handshake synchronized via HAL_UART_TxCpltCallback to prevent DMA stream collisions and buffer overrun during rapid external GPIO trigger events (button toggling).

## Hardware Configuration

* MCU: STM32 (ARM Cortex-M4)
* Serial Interface: USART2 (115200 Baud, 8N1, TX DMA Stream Enabled)
* Input Trigger: User Button on PC13 (Active-Low with Software Debounce)
* Actuator Output: Motor/Load Driver on PA0 (Digital Output, Push-Pull)

## Key Engineering Principles

* Zero-CPU Serialization: HAL_UART_Transmit_DMA offloads memory-to-peripheral byte transfers completely to the DMA controller, immediately freeing the CPU core.
* DMA Resource Guarding: Implements an atomic synchronization flag (dma_ready) to enforce single-flight transfers, ensuring a new transmission cannot overwrite an active DMA stream.
* Hardware Transfer Completion Handshake: HAL_UART_TxCpltCallback serves as an asynchronous completion hook to release the transmission lock once the last frame exits the bus.
* Debounced State Actuation: Robust edge detection and button-release latching preventing multiple trigger firings during contact bounce.

## Operational Execution Flow

1. User presses button on PC13 (debounced via 50 ms filter).
2. Firmware verifies dma_ready == 1 before toggling motor state on PA0.
3. Firmware raises lock (dma_ready = 0) and dispatches the corresponding state string over DMA.
4. CPU continues execution immediately without waiting for serial transmission latency.
5. Upon transfer completion, the DMA hardware invokes HAL_UART_TxCpltCallback, restoring dma_ready = 1.
