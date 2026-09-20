# Non-Blocking UART Telemetry via Timer Interrupts & Transmission Callbacks

This project demonstrates an event-driven telemetry pipeline on an STM32 microcontroller. Instead of stalling the CPU with polling loops (`HAL_Delay` or blocking `HAL_UART_Transmit`), the system leverages hardware timer update interrupts to trigger ADC acquisition and dispatches serial data non-blockingly using UART transmission interrupts.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Timebase Generator:** TIM2 (Configured for periodic 1.0 Hz / 1-second update interrupts)
* **Analog Input:** ADC1 Channel 1 (`PA1` or Potentiometer input, 12-bit)
* **Serial Telemetry:** USART2 (115200 Baud, 8N1, TX Interrupt enabled in NVIC)
* **Status Indicator:** User LED on `PA5`

---

## 🔍 Key Engineering Principles

* **Zero-Overhead Main Loop:** `while(1)` remains entirely empty; system execution is wholly orchestrated by asynchronous peripheral hardware interrupts.
* **Periodic Timebase Trigger:** TIM2 fires `HAL_TIM_PeriodElapsedCallback` every 1000 ms, initiating rapid analog conversion without software timing drift.
* **Non-Blocking Serial Transmission:** `HAL_UART_Transmit_IT` streams telemetry frames in the background via hardware shift registers, immediately releasing CPU context.
* **Hardware Handshake Verification:** The hardware triggers `HAL_UART_TxCpltCallback` once the final byte physically leaves the transmit register, toggling an LED to confirm bus delivery.

---

## 💻 System Telemetry Output

Every 1 second, the host serial terminal receives the formatted voltage reading:

`Voltage: 1.65 V | ADC: 2048`

1. **Voltage:** Linear real-world potential (0.00V - 3.30V)
2. **ADC:** Raw 12-bit sampled magnitude (0 - 4095)
3. **LED Toggle:** Confirms successful UART transmission completion via ISR.
