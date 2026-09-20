# Variable-Length UART Framing via Extended Reception & IDLE Events

This project demonstrates the modern STM32Cube HAL Extended Reception API (`HAL_UARTEx`) to handle arbitrary-length serial data streams. By configuring `HAL_UARTEx_ReceiveToIdle_IT`, the UART hardware continuously monitors the physical RX line, triggering an asynchronous event callback the instant an IDLE line condition is recognized, without requiring manual register manipulation.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1, Global Interrupt Enabled)
* **Status Actuator:** User LED on `PA5`

---

## 🔍 Key Engineering Principles

* **Modern Extended HAL API:** Replaces legacy register polling with `HAL_UARTEx_ReceiveToIdle_IT` and `HAL_UARTEx_RxEventCallback`.
* **Automatic Frame Length Tracking:** The HAL automatically calculates and passes the exact received byte count via the `Size` argument directly to the event callback.
* **Delimiter-Agnostic Ingestion:** Captures data frames without searching for special termination bytes (`\r`, `\n`, or NULL characters).
* **Continuous Event Listening:** Re-arming the extended reception service at the end of each event lifecycle guarantees zero missed frames across sequential bursts.

---

## 💻 Execution Flow

1. The host terminal sends an arbitrary command (e.g., `"PING"` or `"ROCKET_AVIONICS_STATUS_CHECK"`).
2. The USART peripheral clocks data directly into `rx_buffer`.
3. The sender stops transmitting; the RX pin remains HIGH for 1 frame duration (IDLE condition).
4. `HAL_UARTEx_RxEventCallback` fires immediately with the exact `Size` parameter populated.
5. The MCU echoes the payload metrics to the serial console and re-arms the listener.
