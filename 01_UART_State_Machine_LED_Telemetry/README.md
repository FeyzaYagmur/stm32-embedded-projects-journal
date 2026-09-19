# UART State-Machine & Non-Blocking Idle Telemetry

This project implements a finite-state actuator controller interfaced with a host terminal via UART on an STM32 microcontroller. The system monitors asynchronous button inputs to toggle an actuator state while simultaneously tracking idle line timeout periods without stalling the processor.

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1, Polling Mode)
* **Actuator:** User LED on `PA5`
* **Digital Input:** User Button on `PC13` (Active-Low configuration)

---

## 🔍 Key Concepts Covered

* **State Tracking:** Bistable state toggling using debounced GPIO edge detection.
* **Non-Blocking Timeout Gating:** Millisecond-accurate timestamp subtraction using `HAL_GetTick()` to trigger periodic idle messages after 3 seconds of inactivity.
* **Structured Serial Messaging:** Clean terminal status reporting over UART for embedded telemetry.
