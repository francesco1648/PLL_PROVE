# PicoLowLevel

This is the code running on the board inside each module of **Rese.Q**.

## 📦 Module Components

Each module includes the following components:

- **Raspberry Pi Pico W**
- **CAN transceiver**, with **MCP2515** and **TJA1050**
- **Two DC motors**, featuring:
  - **Pololu G2 24v13** driver
  - **Rotary encoder** with **48 pulses per motor rotation**
  - **150:1 gearbox**
- **One to three Dynamixel AX-12A smart servo motors**
- **AMS AS5048B absolute encoder**
- **64×128 OLED display**, with **SH1106 driver**

---

## 🛠️ Building the Project

To build the project, you need a working Arduino environment. You can use:

- The **official Arduino IDE**
- **VSCode**, with the [Arduino extension](https://github.com/microsoft/vscode-arduino)
- Or simply **[arduino-cli](https://github.com/arduino/arduino-cli)**

### 📌 Arduino-Pico

This project is based on the Arduino framework and specifically uses the **Raspberry Pi Pico core**, available [here](https://github.com/earlephilhower/arduino-pico). You can follow the installation guide in the repository's README.

### 📚 Required Libraries

Currently, the only external library used is:

- `Adafruit SH110X` → for controlling the OLED display

It can be installed via the Arduino Library Manager.

### ⚙️ Build Options

In the **Arduino IDE**, select:

- **Board:** Raspberry Pi Pico W
- **Flash Size:** `2MB (Sketch: 1MB, FS: 1MB)`
  - **1MB** for the program code
  - **1MB** for data storage and OTA updates

---

## ⚠️ Before Using the Makefile

🔹 **Before executing any Makefile command, ensure you have installed `arduino-cli`.**

You can download and install it from [here](https://github.com/arduino/arduino-cli).

Before using the Makefile, ensure that you have installed the correct rp2040 core. You can do this by running the following command in your command prompt:

```bash
arduino-cli core install rp2040:rp2040
```

Additionally, to run the Makefile on Windows, you need to install MinGW-w64. MinGW-w64 provides a complete toolchain including the GNU Compiler Collection (GCC) and other essential tools for compiling code on Windows. You can download it from the official [ MinGW-w64 website](https://github.com/niXman/mingw-builds-binaries/releases).
