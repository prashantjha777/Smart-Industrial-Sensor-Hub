# Smart Industrial Sensor Hub
**An Embedded RTOS-based Data Acquisition System**

This project demonstrates a professional-grade embedded application using the **STM32F401RE** (ARM Cortex-M4) and **FreeRTOS**. It focuses on real-time task management, thread-safe communication, and low-level peripheral interfacing.

## 🏗 Architecture & Design
The system is designed with a multi-tasking architecture to ensure high reliability and responsiveness:

* **Sensor Task (I2C):** Interfaces with environmental sensors using the I2C protocol. Implements error checking and data validation.
* **Processing Task:** Handles data scaling and checks for threshold violations (Alarms).
* **Logging Task (UART):** A dedicated gatekeeper task that manages the UART peripheral to provide thread-safe system debugging.

## 🛠 Technical Specifications
* **Microcontroller:** STM32F401RET6 (84 MHz)
* **Kernel:** FreeRTOS (Preemptive Scheduling)
* **Peripherals Used:** * **I2C1:** Master mode for sensor data retrieval.
    * **USART2:** 115200 baud for telemetry and logging.
    * **GPIO:** External interrupts for system reset/user input.
* **Memory Management:** Static allocation for critical tasks to prevent heap fragmentation.

## 🚀 Key Learning Outcomes
* Implemented **Message Queues** for inter-task communication.
* Managed shared resources using **Mutexes** to prevent race conditions.
* Developed a hardware abstraction layer (HAL) for portable peripheral drivers.

## 📂 Project Structure
* `Core/Src`: Contains main application logic and RTOS task definitions.
* `Core/Inc`: Header files and configuration macros.
* `Drivers/STM32F4xx_HAL_Driver`: Standard hardware abstraction layer provided by ST.