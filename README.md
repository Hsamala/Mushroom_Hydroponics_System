# 🍄 Mushroom Growth Hydroponics System (V1.00)
**An Automated Environmental Controller for Mycology**

This project is a environmental control system designed to regulate $CO_2$, humidity, and soil moisture for mushroom cultivation. Built on the **ESP32-S3**, it leverages JTAG debugging, real-time sensor validation, and non-blocking control logic to ensure a stable microclimate for high-yield growth.


## 🛠 Currently Implemented & Working

### ☁️ IoT & Cloud Connectivity (New!)
* **Blynk IoT Integration:** Established a robust, non-blocking bidirectional link between the ESP32-S3 and the Blynk Cloud.
* **Dual-Speed Task Scheduling:** Replaced manual `millis()` logic with `BlynkTimer` to manage high-frequency OLED UI refreshes (5s) and low-frequency cloud data synchronization (3m).

### 🔋 Power & Hardware Architecture
* **High-Current Power Rail:** Verified system stability using a **12V 600mA** external power supply via MB102 Breadboard Dedicated Power Supply Module Compatible 5V 3.3V Adjustable Step-down Module. 
* **Stress-Tested Stability:** Successfully handled peak current draws from simultaneous WiFi radio handshakes and OLED full-pixel refreshes without brownouts.

### 📊 Sensors & UI
* **Intelligent Gas Sensing:** Integrated the **ScioSense ENS160**. Implemented logic to monitor the `DEVICE_STATUS` register to handle Initial Start-up and Warm-up phases, ensuring data validity.
* **Multi-State OLED Interface:** Created a non-blocking UI that cycles through CO2, Temp, Humidity, and Moisture every 10 seconds.
* **Capacitive Soil Sensing:** Calibration-based analog system mapping raw 12-bit ADC values to moisture percentages (Dry ≈ 3500, Wet ≈ 1600).

---

## 🔌 Power & Connectivity Details

To handle the high current demands of pumps, fans, and misters, the system utilizes a dedicated power rail:


* **Power Source:** 12V 600mA External DC Supply. MB102 Breadboard Dedicated Power Supply Module, steps the 12V down into a usuable form to not burn out the ESP32-S3-SuperMini.
* **Regulation:** ESP32-S3 onboard LDO handles logic; High-current rail bypassed to the Relay Matrix. 
* **Connectivity:** 2.4GHz WiFi (Blynk Protocol).
* **Data Strategy:** 3-minute polling interval to optimize Blynk message quotas (200k/month limit).

## 💻 Development Environment & Build Settings
The project is developed using the **Arduino IDE** targeting the **ESP32-S3 Dev Module**.

![Arduino IDE Settings](/images/Screenshot 2025-12-27 at 8.08.41 PM.png)

| Setting | Value |
| :--- | :--- |
| **Board** | ESP32S3 Dev Module |
| **USB CDC On Boot** | Enabled |
| **CPU Frequency** | 240MHz (WiFi) |
| **Flash Mode / Size** | QIO 80MHz / 4MB (32Mb) |
| **USB Mode** | Hardware CDC and JTAG |
| **Upload Mode** | UART0 / Hardware CDC |

## 🛠 Build & Debugging Guide

### 1. Toolchain Setup
* **Compiler**: `Arduino IDE ESP-ELF Toolchain`
* **Debugger**: `xtensa-esp-elf-gdb` (v11.2)
* **JTAG Interface**: OpenOCD (Open On-Chip Debugger)

### 2. Hardware Debugging Workflow 🐛
This project utilizes the ESP32-S3's built-in USB-JTAG peripheral. To troubleshoot fatal crashes:

1.  **Launch OpenOCD** (Server):
    ```bash
    openocd -c "set ESP_RTOS none" -f board/esp32s3-builtin.cfg
    ```
2.  **Launch GDB** (Client):
    ```bash
    xtensa-esp-elf-gdb Mushroom_Growth_Project_V0.50.ino.elf
    ```
3.  **Establish Handshake**:
    * `target remote :3333` — Connect to hardware.
    * `monitor reset halt` — Pause CPU at start.
    * `break loop` — Set breakpoint at main logic.
    

## 🚀 Future Roadmap
* ✅ **MVP Relay Matrix**: Implement **Hysteresis** triggers for Fan/Mister.
* 🏗 **Environmental Hardening**: Move to permanent soldered board with moisture protection.
* 🧪 **CI/CD Integration**: Automate logic verification using GitHub Actions and Unity.
* 🔋 **Power Management**: Transition to LiPo power with light-sleep optimization.
