# Asynchronous UART String Command Parser (CLI) with Dynamic Memory Safety

This project implements a multi-byte ASCII Command Line Interface (CLI) over UART on an STM32 microcontroller. Instead of single-character triggers, the system buffers incoming byte streams via RX interrupts until a line delimiter is detected, signals the main thread via an atomic flag, and safely parses structured instructions using standard C string operations.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1, RX Interrupt enabled)
* **Actuator:** User LED on `PA5` (Digital Output, Push-Pull)

---

## 🔍 Key Engineering Principles

* **Decoupled ISR Architecture:** The interrupt service routine (`HAL_UART_RxCpltCallback`) only handles byte storage, buffer bounds checking, and delimiter recognition. String parsing is offloaded to the main loop to maintain minimal ISR latency.
* **Buffer Overflow Protection:** Guard conditions prevent index increment beyond buffer bounds (`RX_BUFFER_SIZE - 1`), avoiding memory corruption.
* **String Analysis Operations:**
  * `strstr`: Substring prefix matching for dynamic arguments (`SET:LED_ON`, `SET:LED_OFF`).
  * `strcmp`: Exact token matching for interrogation queries (`STATUS`).
* **Safe State Reset:** Systematic zero-fill using `memset` and index reset prevents dirty-read artifacts from lingering between packet frames.

---

## 💻 Supported Command Set

1. **`SET:LED_ON`**: Activates LED (`PA5` HIGH) and transmits `[OK] LED Turned ON.`
2. **`SET:LED_OFF`**: Deactivates LED (`PA5` LOW) and transmits `[OK] LED Turned OFF.`
3. **`STATUS`**: Reads current `PA5` pin level and returns formatted telemetry (`[STATUS] LED State: ACTIVE / INACTIVE`).
4. **Invalid Command**: Emits formatted error banners (`[ERR] Invalid SET parameter!` or `[ERR] Unknown Command!`).
