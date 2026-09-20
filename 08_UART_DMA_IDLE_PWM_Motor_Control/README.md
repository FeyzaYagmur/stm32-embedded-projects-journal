# Zero-Overhead UART Control: DMA IDLE Line Reception & Dynamic PWM Actuation

This project demonstrates an asynchronous, high-efficiency hardware control interface on an STM32 microcontroller. Variable-length ASCII commands are ingested directly into SRAM using Direct Memory Access (DMA) paired with the UART Hardware IDLE line event. The CPU remains completely unburdened during data reception, parsing incoming target setpoints only upon transmission completion to dynamically update hardware PWM duty cycles (e.g., motor speed or LED brightness).

---

## ⚙️ Hardware Configuration

* **MCU:** STM32 (ARM Cortex-M4)
* **Serial Interface:** USART2 (115200 Baud, 8N1, Global Interrupt Enabled)
* **DMA Controller:** DMA1 Stream 5 Channel 4 (USART2_RX, Normal Mode)
* **Actuator Output:** TIM2 Channel 1 (`PA0` or `PA5` depending on pinout, ARR = 1000)
* **Heartbeat Indicator:** User LED on `PA5`

---

## 🔍 Key Engineering Principles

* **Zero-CPU Serialization:** Ingesting bytes via `HAL_UARTEx_ReceiveToIdle_DMA` offloads memory operations entirely to the DMA controller.
* **Variable-Length Packet Framing:** Triggered automatically by the hardware IDLE line condition when the transmitter ceases data transfer.
* **Dynamic String Tokenization:** 
  * `strstr`: Detects instruction keywords (`STOP` and `SPEED`).
  * `atoi`: Parses numeric string values directly into integer PWM duty cycle metrics.
* **Boundary Validation & Failsafe:** Enforces a 0 to 1000 duty cycle range ceiling to prevent timer compare register overflow.
* **Safe DMA Re-arming:** Flushing buffers via `memset` and re-invoking the reception service inside the callback maintains continuous readiness for subsequent bursts.

---

## 💻 Supported Command Set

1. **`SPEED <value>`**: Sets PWM Compare Register to `<value>` (Range: `0` to `1000`).  
   *Example:* `SPEED 750` $\rightarrow$ Updates duty cycle to 75% and echoes confirmation.
2. **`STOP`**: Resets PWM Compare Register to 0 (Motor/Actuator Stop) and transmits confirmation.
3. **Out-of-Range / Malformed Input:** Retains prior hardware state without applying invalid values.
