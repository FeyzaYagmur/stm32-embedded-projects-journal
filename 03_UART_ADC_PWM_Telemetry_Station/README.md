# ADC Data Acquisition, Hardware PWM Actuation & UART Telemetry

This project implements an integrated control loop combining analog sensing, hardware timer PWM output, and serial telemetry on an STM32 microcontroller. The system samples an analog voltage using the ADC, maps the result to adjust PWM duty cycle, and formats real-time metrics using standard C string utilities for UART transmission.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Analog Input:** ADC1 Channel 1 (`PA1` or Potentiometer input)
* **PWM Output:** TIM4 Channel 1 (`PD12`, Auto-Reload Register set to 1000)
* **Serial Telemetry:** USART2 (115200 Baud, 8N1)

---

## 🔍 Key Engineering Principles

* **Analog-to-Digital Conversion (Polling):** Reading 12-bit raw conversion values (0 to 4095) with hardware flag monitoring via `HAL_ADC_PollForConversion`.
* **Hardware PWM Scaling:** Linearly mapping 12-bit input samples to the timer compare register (`__HAL_TIM_SET_COMPARE`) to dynamically modulate actuator power or LED dimming.
* **Safe Buffer Formatting:** Utilizing `memset` to clear memory and `sprintf` to encode analog metrics into a structured telemetry frame.
* **Dynamic Transmission Sizing:** Employing `strlen` with `HAL_UART_Transmit` to send only active string bytes without trailing empty buffer overhead.

---

## 💻 Telemetry Data Frame

The system outputs a periodic log every 1000 ms in the following structure:

`[TELEMETRY] ADC: 2048 | Voltage: 1.65 V | PWM: 500`

1. **ADC:** Raw 12-bit digital conversion (0 - 4095)
2. **Voltage:** Converted real-world analog potential (0.00V - 3.30V)
3. **PWM:** Timer compare value written to CCR1 register (0 - 1000)
