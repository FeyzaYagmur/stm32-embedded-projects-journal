# UART Interactive Command Interface & Polling Timeout Control

This project demonstrates a single-byte command-line parser implemented via UART using polling reception on an STM32 microcontroller. The primary focus is handling the peripheral timeout parameter within `HAL_UART_Receive` to prevent processor lockup while interacting with physical actuators (LED and Buzzer).

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1)
* **Actuator 1 (LED):** `PA5` (Digital Output, Push-Pull)
* **Actuator 2 (Buzzer / Aux LED):** `PA6` (Digital Output, Push-Pull)

---

## 🔍 Key Engineering Principles

* **Bounded Polling Reception:** Avoiding `HAL_MAX_DELAY` by assigning a finite 50 ms timeout window, enabling the CPU to service background routines without hanging on absent serial input.
* **Instruction Parsing:** Decoding incoming single-byte frames (`1`, `0`, `b`/`B`) to actuate external circuits with real-time serial feedback echoes.
* **CR/LF Sanitization:** Filtering Carriage Return (`\r`) and Line Feed (`\n`) bytes commonly appended by terminal emulators.
* **Error Containment:** Detecting and reporting undefined instructions via dedicated invalid command prompts.

---

## 💻 Terminal Command Interface

1. **Command '1':** Turn ON LED (Sets PA5 pin to HIGH)
2. **Command '0':** Turn OFF LED (Sets PA5 pin to LOW)
3. **Command 'b' or 'B':** Buzzer Chirp (Sets PA6 pin to HIGH for 200 ms pulse)
4. **Any other key:** Invalid Input (Returns error warning message)
