# Interrupt-Driven UART Command Parser & Hardware PWM Actuation

This project demonstrates a full-duplex, non-blocking hardware control interface on an STM32 microcontroller. The main execution thread maintains an independent visual heartbeat routine while asynchronous UART reception interrupts capture serial control bytes in the background to dynamically adjust hardware PWM timer outputs (e.g., servo positions or LED dimming) with instant interrupt-driven feedback.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1, RX & TX Interrupts enabled in NVIC)
* **PWM Output:** TIM4 Channel 1 (`PD12`)
* **Visual Heartbeat:** User LED on `PA5`

---

## 🔍 Key Engineering Principles

* **Asynchronous Command Ingestion:** Utilizes `HAL_UART_Receive_IT` to trap incoming single-byte commands without processor stalling.
* **Continuous Interrupt Arming:** Re-arming the RX interrupt pipeline inside `HAL_UART_RxCpltCallback` to ensure non-stop serial listening across successive bytes.
* **Independent Execution Contexts:** The `while(1)` background loop independently toggles a heartbeat LED every 500 ms, completely decoupled from serial transmission and PWM handling.
* **Dynamic Hardware Modulation:** Adjusting the timer compare register (`__HAL_TIM_SET_COMPARE`) on the fly based on parsed command characters.

---

## 💻 Command Set & Hardware Actions

1. **Command 'A' / 'a':** Sets PWM Compare Register to 1250 (25% Duty / Position A) and transmits acknowledgement.
2. **Command 'B' / 'b':** Sets PWM Compare Register to 1750 (75% Duty / Position B) and transmits acknowledgement.
3. **Command 'S' / 's':** Sets PWM Compare Register to 1000 (Safe Default / Reset Position) and transmits confirmation.
4. **Any Undefined Key:** Dispatches an error prompt over UART without affecting the active PWM state.
