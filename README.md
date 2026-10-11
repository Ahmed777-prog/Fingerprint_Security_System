# RTOS-Based Biometric Access Control System | ATmega32

[![Platform](https://img.shields.io/badge/Platform-ATmega32%20(8MHz)-blue.svg)]()
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green.svg)]()
[![Language](https://img.shields.io/badge/Language-Embedded%20C-orange.svg)]()

A robust, production-grade Biometric Access Control System engineered on a resource-constrained 8-bit ATmega32 microcontroller (2KB SRAM) using FreeRTOS. The system provides real-time fingerprint authentication, dynamic user role management, physical door actuation, and an on-demand administrative console.

---

## ⚡ Key Engineering & Architectural Highlights

- **Multi-Tasking Kernel & Deterministic IPC:**
  - Implemented the **Producer-Consumer pattern** via FreeRTOS Queues (`xQueueCreate(3, sizeof(u16))`).
  - Prioritized physical acquisition (`Task_SensorMonitor`, Priority 3) for zero-latency user detection.
  - Decoupled physical actuation (`Task_SecurityLogic`, Priority 2) using `portMAX_DELAY` for **0% idle CPU utilization**.
  - On-demand control console (`Task_AdminUI`, Priority 1) using deterministic lifecycle transitions (`vTaskSuspend` / `vTaskResume`).

- **2KB SRAM Optimization (Zero-RAM UI):**
  - Eliminated runtime stack allocations and silent memory overflows by storing 100% of static UI and debugging strings into Flash ROM using `PROGMEM` and custom Flash-reading LCD routines (saving 350+ bytes of static RAM).
  - Devised an **On-The-Fly Zero-RAM User Browser** that directly queries records dynamically from external I2C EEPROM on user input, eliminating runtime array allocations inside task stacks.

- **Dual-Storage Biometric Synchronization & Security:**
  - Synchronized the R307 Optical Fingerprint Sensor (UART 57600 baud) with an external 24C08 I2C EEPROM for persistent access logs and privileges.
  - Role-Based Access Control (Admin vs. Standard User).
  - Built-in hardware-independent admin immunity safeguarding the primary administrator (`ID: 0`) against accidental deletion.

- **Power Integrity & Inductive Decoupling:**
  - Stabilized the power rails against SG90 servo motor inrush stall currents by integrating a **1000µF decoupling capacitor**, completely eliminating MCU brown-out resets and LCD data bus corruption.

---

## 🏗 System Architecture & Tasks

| Task Name | Priority | Stack Depth | State | Function |
| :--- | :---: | :---: | :--- | :--- |
| **`Task_SensorMonitor`** | 3 (High) | 160 Words | Blocked / Polling | Monitors R307 sensor and 'A' key; produces fingerprint events to Queue. |
| **`Task_SecurityLogic`** | 2 (Medium) | 140 Words | Blocked on Queue | Consumes IDs, checks EEPROM privileges, and drives the Servo latch. |
| **`Task_AdminUI`** | 1 (Low) | 160 Words | Suspended | Handles user enrollment, on-the-fly deletion, and exits cleanly. |

---

## 🔌 Hardware Mapping & Pinout

| Microcontroller Pin | Peripheral Device | Protocol / Interface | Function |
| :--- | :--- | :--- | :--- |
| **PD0 (RXD)** | R307 Fingerprint Sensor | UART (57600 bps) | Data reception from optical module |
| **PD1 (TXD)** | R307 Fingerprint Sensor | UART (57600 bps) | Command transmission |
| **PC0 (SCL)** | 24C08 External EEPROM | I2C / TWI (100 kHz) | Clock line |
| **PC1 (SDA)** | 24C08 External EEPROM | I2C / TWI (100 kHz) | Data line |
| **PD5 (OC1A)** | SG90 Servo Motor | 50Hz PWM (Timer1) | Door latch actuation (0° to 90°) |
| **PD6** | Red LED Indicator | GPIO Output | Unauthorized access / System alert |
| **PD7** | Green LED Indicator | GPIO Output | Authorized access / Admin mode |
| **PORTA (PA0–PA7)** | 16x2 Character LCD | 4-bit Parallel Mode | User feedback and admin UI |
| **PORTB (PB0–PB7)** | 4x4 Matrix Keypad | Matrix Scanning | Keypad input |