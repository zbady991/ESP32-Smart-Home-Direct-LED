# 🚀 ESP32-Smart-Home-Direct-LED

**Production-Grade Smart Home Lighting Controller**
*Powered by ESP32 & ESP RainMaker Ecosystem — Direct LED Architecture*

An advanced, highly stable, and hybrid IoT smart home solution built around the **ESP32** microcontroller and the **ESP RainMaker** cloud infrastructure. The system supports concurrent manual override via physical switches, real-time cloud synchronization, voice control, and persistent state storage — all running in true dual-mode (offline + online) without any single point of failure.

> Originally developed as a Robotics Course mini-project at **Pharos University** (Faculty of Computer Science & AI), and rebuilt here as a polished, standalone IoT system.

---

## 📌 The Problem

Most basic smart-home setups suffer from:
- ❌ No real-time sync between offline physical buttons and the cloud app
- ❌ Electrical noise on hardware inputs causing false triggers (no debouncing)
- ❌ Device states getting wiped on every power cycle
- ❌ Total dependency on internet — local switches become useless during network drops

## ✅ The Solution: Dual-Mode Architecture

This project solves all of the above with a **zero-downtime, dual-mode design**:

| Mode | Trigger | Internet Required | Latency |
|---|---|---|---|
| **Offline / Local** | Physical push buttons | ❌ No | < 10 ms |
| **Online / Cloud** | RainMaker App | ✅ Yes | ~100–300 ms |
| **Voice** | Google Home / Alexa | ✅ Yes | ~500 ms – 1 s |

Both modes run **simultaneously**. Whichever input fires first updates the LED, writes to EEPROM, and reports the new state back to the cloud — keeping the physical world and the app perfectly in sync at all times.

---

## ⚙️ Key Features

- 🔌 **4 independently controlled LEDs** via app, voice, and physical push buttons
- ☁️ **Real-time cloud sync** between hardware state and the RainMaker app
- 💾 **EEPROM-based state persistence** — survives power cycles and reboots
- 🗣️ **Voice control** via Google Home & Amazon Alexa (through RainMaker)
- 📶 **BLE Wi-Fi provisioning** with QR code setup — no hardcoded credentials
- 🔁 **Auto-reconnect** logic with zero downtime on Wi-Fi drops
- 🛠️ **Debounced hardware inputs** using the AceButton library (no extra RC filters needed)
- 🔄 **OTA updates** support
- 🧱 **Factory reset / Wi-Fi reset** via the BOOT button (hold 3s / 10s)
- 🧩 Modular codebase, ready for sensor/relay expansion (see Roadmap)

---

## 🏗️ System Architecture

```
 Offline / Local Mode                      Online / Cloud Mode
 ┌────────────────────┐                  ┌────────────────────┐
 │ 4x Physical Buttons │                  │ RainMaker App +     │
 │  (AceButton)         │                  │ Voice Assistants    │
 └─────────┬───────────┘                  └─────────┬───────────┘
           │                                          │ Wi-Fi
           ▼                                          ▼
        ┌────────────────────────────────────────────────┐
        │              ESP32 Microcontroller              │
        │   write_callback() ⇄ EEPROM ⇄ Hardware GPIO     │
        └─────────────────────┬────────────────────────────┘
                               ▼
                          4x LED Output
```

**Software layers:**
1. **Non-Volatile Memory** — EEPROM persistence loop (addresses 0–3) restores state before provisioning even begins
2. **Input Handling** — AceButton library (v1.10.1) handles debouncing & triggers on button release
3. **Connection & Provisioning** — WiFiProv (BLE) for QR-code Wi-Fi setup
4. **Cloud Integration Layer** — ESP RainMaker registers each LED as a `Switch` device; `write_callback()` handles ON/OFF commands from the app/voice

---

## 🔧 Hardware

| Component | Pin | Notes |
|---|---|---|
| LED 1–4 (Outputs) | GPIO 23, 22, 21, 19 | Via 220Ω current-limiting resistors |
| Push Buttons 1–4 (Inputs) | GPIO 13, 12, 14, 27 | Momentary, `INPUT_PULLUP`, one leg to GND |
| Wi-Fi Status Indicator | GPIO 2 | ON when connected |
| Factory/Wi-Fi Reset | GPIO 0 (BOOT) | Hold 3s = Wi-Fi reset, 10s = factory reset |
| Power | 5V USB | 3.3V logic |

📷 Full wiring diagram: [`circuit diagram.png`](./circuit%20diagram.png)

---

## 📚 Tech Stack

- **MCU:** ESP32
- **Framework:** Arduino IDE 2.x + ESP32 board package (v2.0.3)
- **Cloud:** ESP RainMaker
- **Libraries:** `EEPROM.h`, `RMaker.h`, `WiFi.h`, `WiFiProv.h`, `AceButton` (v1.10.1)
- **Voice Integration:** Google Home / Amazon Alexa (via RainMaker)

---

## 🚀 Getting Started

1. Install **Arduino IDE 2.x**, the **ESP32 board package (v2.0.3)**, and the **AceButton** library
2. Set **Partition Scheme** to `RainMaker`
3. On first upload, enable **Erase All Flash** (prevents legacy NVS corruption)
4. Wire the 4 LEDs + 4 buttons per the pin table above
5. Flash `v2.ino` at **115200 baud**
6. Open Serial Monitor → scan the generated **BLE provisioning QR code** with the ESP RainMaker app
7. Pass your Wi-Fi credentials securely over BLE
8. (Optional) Link your RainMaker node to Google Home / Alexa from the app settings

---

## ✅ Results & Validation

| Test | Result |
|---|---|
| App Control | ✅ Pass |
| Local/Physical Control | ✅ Pass |
| Offline Functionality | ✅ Pass |
| EEPROM State Restoration | ✅ Pass |
| Wi-Fi Drop Recovery | ✅ Pass |
| Google Home Voice Control | ✅ Pass |
| Amazon Alexa Voice Control | ✅ Pass |
| Factory Reset | ✅ Pass |

**Overall: 100% reliability across all 8 core test metrics.**

---

## 🗺️ Roadmap (v2.0)

- 🔆 **PIR Motion Sensor (HC-SR501)** — auto-trigger LEDs on motion, auto-off after 30s idle
- 🌀 **DC Fan Ventilation** — integrated as a 5th cloud-controlled device
- 🔌 **4-Channel Relay Module** — replace LEDs with real high-voltage appliance switching

---
## 📄 Documentation

- [Project Presentation (PDF)](./Smart_Home_Presentation.pdf)
- [Circuit Diagram](./circuit%20diagram.png)

---

## 👤 Author

**Abdulrhman Mohammed** — [@zbady991](https://github.com/zbady991)

*Built as part of the Robotics Course at Pharos University, Faculty of Computer Science & AI — Academic Year 2025/2026.*

## 📜 License

This project is open-sourced under the MIT License.
