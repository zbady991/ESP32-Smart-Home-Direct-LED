# ESP32-Smart-Home-Direct-LED
Advanced ESP32 Smart Home solution integrated with ESP RainMaker, featuring direct LED architecture and hybrid manual controls.
# 🚀 Production-Grade Smart Home Lighting Controller
### Powered by ESP32 & ESP RainMaker Ecosystem (Direct LED Architecture)

An advanced, highly stable, and hybrid IoT smart home solution designed around the **ESP32** microcontroller and **ESP RainMaker** cloud infrastructure. This system features concurrent manual override via physical switches, real-time cloud synchronization, and persistent state management, driving lighting loads directly.

---

## 📂 Project Documentation & Presentation

📊 **[Click Here to View the Full Project Presentation (PDF)](./Smart_Home_Presentation.pdf)** *(You can read the full architectural details, system design, and hardware specifications directly in this presentation file without downloading).*

---

## 🔌 Hardware Circuit Diagram

Below is the optimized hardware schematic illustrating the direct LED driving connection and switch matrices, fully cleared of any relay decoupling modules.

![Hardware Circuit Diagram](./circuit_diagram.png)

---

## 🏗️ System Architecture & Workflow

The system is architected to operate seamlessly in multi-input environments, ensuring zero blockages between cloud commands and physical switch interrupts.
تفضل يا بشمهندس عبد الرحمن، ده ملف الـ README.md كامل وجاهز دفعة واحدة. الملف ده متعدل ومربوط فيه حل مشكلة الـ PDF باستخدام الـ ?raw=true عشان يفتح مع أي حد علطول بدون تهنيج.  انسخ الكود اللي في المربع الأسود ده بالكامل واعمل بيه استبدال (Replace) للملف القديم على GitHub:  Markdown# 🚀 Production-Grade Smart Home Lighting Controller
### Powered by ESP32 & ESP RainMaker Ecosystem (Direct LED Architecture)

[cite_start]An advanced, highly stable, and hybrid IoT smart home solution designed around the **ESP32** microcontroller and **ESP RainMaker** cloud infrastructure[cite: 61, 67, 68, 92]. [cite_start]This system features concurrent manual override via physical switches, real-time cloud synchronization, and persistent state management, driving lighting loads directly[cite: 61, 64, 69, 72, 73, 94].

---

## 📂 Project Documentation & Presentation

📊 **[Click Here to View the Full Project Presentation (PDF)](./Smart_Home_Presentation.pdf?raw=true)** *(You can read the full architectural details, system design, and hardware specifications directly in this presentation file without downloading).*

---

## 🔌 Hardware Circuit Diagram

Below is the optimized hardware schematic illustrating the direct LED driving connection and switch matrices, fully cleared of any relay decoupling modules.

![Hardware Circuit Diagram](./circuit_diagram.png)

---

## 🏗️ System Architecture & Workflow

[cite_start]The system is architected to operate seamlessly in multi-input environments, ensuring zero blockages between cloud commands and physical switch interrupts[cite: 72, 73, 78, 80].

              ┌────────────────────────┐
              │   ESP RainMaker Cloud  │
              └───────────┬────────────┘
                          │ (Wi-Fi / BLE Provisioning)
                          ▼
┌──────────────┐    ┌──────────────────┐│ Push Buttons ├───►│  ESP32 Firmware  │ ──► [EEPROM Backup]│ (AceButton)  │    │  (State Machine) │└──────────────┘    └─────────┬────────┘│▼┌──────────────────┐│ 4-Channel LEDs   │└──────────────────┘
## ⚡ Technical Highlights & Implementation Details

### 1. Hybrid Control Architecture (Asynchronous Flow)
* **The Challenge:** Preventing hardware button polling from blocking network requests, and vice versa[cite: 72, 73, 78, 80].
* **The Solution:** Integrated the `AceButton` library using event-driven callback mechanisms rather than traditional blocking `digitalRead()` loops[cite: 61, 66, 72, 73, 88, 89, 90, 91]. This ensures immediate responsiveness (<10ms latency) even during active cloud synchronization[cite: 72, 73, 88, 94].

### 2. Fault Tolerance & Non-Volatile Memory (NVM)
* Utilizes the ESP32’s **EEPROM emulation layer** to achieve robust state persistence[cite: 61, 69, 70, 85].
* Every state mutation (Cloud or Manual Button) commits the exact byte configurations to the NVM[cite: 69, 70, 72, 80]. Upon unexpected power cycles, the system boots into its exact pre-failure hardware state within milliseconds[cite: 85, 86, 87, 88].

### 3. Advanced Provisioning & Fleet Management
* Employs **Unified Provisioning via Bluetooth Low Energy (BLE)**[cite: 61, 73, 74, 93].
* Includes an intelligent physical reset logic attached to GPIO 0 (BOOT button)[cite: 65, 87, 95, 96]:
  * **Short Press (3s < t < 10s):** Triggers `RMakerWiFiReset()` to re-configure network credentials without wiping user data[cite: 95, 96, 98].
  * **Long Press (t > 10s):** Triggers `RMakerFactoryReset()` to fully wipe NVM storage and restore factory defaults[cite: 95, 96, 97].

---

## 📊 Hardware Pin Mapping (GPIO Configurations)

| Peripheral Component | ESP32 Pin | Function | Hardware State |
| :--- | :---: | :--- | :--- |
| **LED Channel 1** | `GPIO 23` | Output | Digital Active HIGH [cite: 63, 86] |
| **LED Channel 2** | `GPIO 22` | Output | Digital Active HIGH [cite: 63, 86] |
| **LED Channel 3** | `GPIO 21` | Output | Digital Active HIGH [cite: 63, 86] |
| **LED Channel 4** | `GPIO 19` | Output | Digital Active HIGH [cite: 63, 86] |
| **Physical Button 1** | `GPIO 13` | Input | Internal Pull-Up Enabled [cite: 64, 87] |
| **Physical Button 2** | `GPIO 12` | Input | Internal Pull-Up Enabled [cite: 64, 87] |
| **Physical Button 3** | `GPIO 14` | Input | Internal Pull-Up Enabled [cite: 64, 87] |
| **Physical Button 4** | `GPIO 27` | Input | Internal Pull-Up Enabled [cite: 64, 87] |
| **Network Status LED** | `GPIO 2` | Output | Connection Indicator [cite: 65, 87] |
| **Hardware Reset Switch** | `GPIO 0` | Input | BOOT Button Interface [cite: 65, 87] |

---

## 🛠️ Software Dependencies & Environment

To compile this firmware, ensure your development environment has the following dependencies installed[cite: 61]:

* **Framework:** Arduino IDE (v2.0+) or PlatformIO
* **Core:** ESP32 Arduino Core v2.X.X
* **External Libraries:**
  * `AceButton` (v1.10.1 or higher) by Brian Park [cite: 61]
  * `EEPROM` (Built-in) [cite: 61]
  * `RMaker` (Espressif Native IoT SDK) [cite: 61]

---

## 🚀 Quick Start Deployment Guide
