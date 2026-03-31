# 🎨 WallAce — Autonomous Wall Painting Bot

> A semi-autonomous robot designed to paint the exteriors of tall buildings, eliminating the safety risks and inefficiencies of traditional manual painting.

Developed by **Team 09** as part of **Post INDUCTION'24** — The Robotics Club, SNIST  
Sreenidhi Institute of Science and Technology, Hyderabad | 2023–2024

---

## 📋 Table of Contents
- [Problem Statement](#-problem-statement)
- [Our Solution](#-our-solution)
- [Demo](#-demo)
- [Hardware Components](#-hardware-components)
- [Software Used](#-software-used)
- [System Architecture](#-system-architecture)
- [Working Mechanism](#-working-mechanism)
- [Source Code](#-source-code)
- [Future Scope](#-future-scope)
- [Team](#-team)
- [Mentors](#-mentors)
- [References](#-references)
- [License](#-license)

---

## 🚧 Problem Statement

Painting the exteriors of tall buildings presents serious challenges in the construction and maintenance industries:

- Workers must hang from great heights, increasing the risk of accidents and injuries
- Manual processes are labor-intensive and lead to uneven paint application
- Weather conditions and worker fatigue cause delays
- High costs due to extended time and manpower requirements
- No scalable, safer automated solution currently exists for high-rise painting

---

## 💡 Our Solution

**WallAce** is a semi-autonomous wall-painting bot consisting of two coordinated components:

- **X-axis component** — moves horizontally along the wall using a rack and pinion mechanism driven by a NEMA 17 stepper motor
- **Y-axis component** — hangs vertically and performs the actual painting using a paint roller, servo motor, and spring mechanism powered by another NEMA 17 stepper motor

Two **ESP32 microcontrollers** communicate wirelessly via **ESP-NOW**, eliminating the need for physical wiring between the two units. When the vertical unit finishes painting one lane, an ultrasonic sensor detects the end of the wall and signals the horizontal unit to shift forward. This continues until the entire wall is uniformly painted.

---

## 🎥 Demo

>

---

## 🔧 Hardware Components

| Component | Description |
|---|---|
| ESP32 × 2 | Dual microcontrollers with built-in Wi-Fi & Bluetooth for wireless communication |
| NEMA 17 Stepper Motors × 2 | Precision motors for X-axis and Y-axis movement |
| Motor Driver A4988 × 2 | Controls the NEMA 17 stepper motors |
| Motor Driver L298N | Controls the center-shaft DC motor |
| Servo Motors × 2 | Controls paint roller dipping and gate mechanism |
| Ultrasonic Sensor (HC-SR04) | Detects end of wall to trigger X-axis shift |
| Paint Roller | Applies paint uniformly on the wall surface |
| Rack and Pinion | Converts rotational motion to linear motion for roller movement |
| Pulley System | Enables smooth vertical movement along the wall |
| Buck-Boost Converter | Regulates voltage between components |
| Capacitor | Stabilizes power supply |
| Coil Spring | Maintains roller pressure and enables retraction |
| Lead-Acid Battery | Power supply (6V) |
| Chassis (Wood) | Structural base for mounting all components |
| Breadboard | Prototyping and connections |
| Rainbow Wires | Flat ribbon cable for neat internal wiring |
| Wheels | For mobility and navigation |

---

## 💻 Software Used

| Software | Purpose |
|---|---|
| [Arduino IDE](https://www.arduino.cc/en/software) | Writing and uploading C++ code to ESP32 boards |
| [Fusion 360](https://www.autodesk.com/products/fusion-360) | 3D CAD modelling and simulation of the bot |
| [Fritzing](https://fritzing.org/) | Circuit schematic and wiring diagram design |

---

## 🏗 System Architecture

```
![BlockDiagram] docs/BlockDiagram.jpg

```

**Communication Flow:**
- Y-component ESP32 detects wall end via ultrasonic sensor
- Sends signal wirelessly (ESP-NOW) to X-component ESP32
- X-component shifts the bot horizontally (500 steps)
- Y-component resets to top and resumes painting the next lane

---

## ⚙️ Working Mechanism

1. **Paint Dipping** — The gate servo opens, allowing the roller to dip into the paint tray for 3 seconds, then closes.

2. **Painting (Y-axis)** — The rack and pinion system moves the roller forward against the wall. The bot alternates the center-shaft motor forward and backward every 3 seconds for 15 seconds, applying paint uniformly in the vertical lane.

3. **Wall-End Detection** — The HC-SR04 ultrasonic sensor checks the distance. If distance < 10 cm (wall end detected), it sends a message to the X-component ESP32.

4. **Horizontal Shift (X-axis)** — Upon receiving the signal, the X-axis stepper motor moves 500 steps horizontally, shifting the bot to the next unpainted lane.

5. **Reset** — The Y-axis motor returns to the starting (top) position.

6. **Repeat** — Steps 1–5 repeat until the entire wall is painted.

---

## 📂 Source Code

The project has two separate ESP32 programs in `src/`:

| File | Description |
|---|---|
| [`src/x_component.ino`](src/x_component.ino) | X-axis ESP32 — receives signal, shifts horizontally |
| [`src/y_component.ino`](src/y_component.ino) | Y-axis ESP32 — paints, detects wall end, sends signal |

### Key Pin Definitions — X-Component

```cpp
#define STEP_PIN_X 14    // X-axis stepper step
#define DIR_PIN_X  12    // X-axis stepper direction
#define STEP_PIN_Y 13    // Y-axis stepper step
#define DIR_PIN_Y  15    // Y-axis stepper direction

const int dis = 200;         // Steps per lane (200 steps ≈ 15 cm)
const int wait_time = 15000; // Time between Y-axis moves (15 sec)
```

### Key Pin Definitions — Y-Component

```cpp
#define IN1 25            // Motor driver IN1
#define IN2 26            // Motor driver IN2
#define ENA 33            // Motor enable
#define TRIG_PIN 5        // Ultrasonic trigger
#define ECHO_PIN 18       // Ultrasonic echo
#define THRESHOLD 10      // Wall detection distance (cm)

// Servo pins: gate → pin 21, roller → pin 20
```

### How to Upload

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install ESP32 board support via Board Manager
3. Install required libraries: `AccelStepper`, `ESP32Servo`, `esp_now`, `WiFi`
4. **First**, update the `slaveAddress[]` in `y_component.ino` with the MAC address of your X-component ESP32
5. Upload `x_component.ino` to the X-axis ESP32
6. Upload `y_component.ino` to the Y-axis ESP32
7. Power both units and the bot will begin operation

> **Note:** To find the MAC address of your X-component ESP32, upload a simple sketch that prints `WiFi.macAddress()` to the Serial Monitor.

---

## 🔮 Future Scope

- Integration of **AI and machine learning** for real-time surface analysis and adaptive painting techniques
- Support for **curved and irregular wall surfaces**
- Improved **battery efficiency** for longer operational time without recharging
- Evolution into a **multi-functional maintenance bot** capable of cleaning, inspecting, and repairing surfaces
- **Collaboration with drones** or robotic scaffolding for fully automated exterior maintenance
- Compliance with **global regulatory and environmental standards** for worldwide deployment

---

## 👥 Team

**Team 09 — Post INDUCTION'24, The Robotics Club - SNIST**

| Name | Branch | Email |
|---|---|---|
| Ch. Manvith | CSE | 23311A0525@cse.sreenidhi.edu.in |
| B. Induja | CSE | 23311A05M1@cse.sreenidhi.edu.in |
| T. Abhishek | CSE | 23311A05R5@cse.sreenidhi.edu.in |
| S. Ashish | CSE | 23311A0502@cse.sreenidhi.edu.in |
| K. Dhakshinya | CSE | 23311A05Q8@cse.sreenidhi.edu.in |
| V. Vaibhavi | CSE | 23311A05E7@cse.sreenidhi.edu.in |
| T. Venkat Prasad | AI & ML | 23311A6632@aiml.sreenidhi.edu.in |

**Executive:** T. Nikhil

---

## 🧑‍🏫 Mentors

- **Mr. B. Navyahaas** & **Mr. P. Phani Anirudh** — Joint Secretaries of Technical Affairs
- **Mr. G. Kovidh Addhish** — President, The Robotics Club - SNIST
- **Dr. A. Purushotham** — Faculty Advisor, Mechanical Department

---

## 📚 References

1. Indian Journal of Science and Technology (IJST) — *Development of Exterior Wall Painting Robot* ([DOI: 10.17485/ijst/2016/v9i38/102580](https://doi.org/10.17485/ijst/2016/v9i38/102580))
2. Journal of Image Processing and Intelligent Remote Sensing (JIPIRS) — *Automatic Wall Painting Robot* ([DOI: 10.55529/jipirs.26.11.22](https://doi.org/10.55529/jipirs.26.11.22))
3. OutoBot — [phys.org article](https://phys.org/news/2017-03-outobot-robot-high-rise.html)

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

*Submitted to The Robotics Club - SNIST as part of Post INDUCTION'24 | 2023–2024*