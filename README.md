# DualBridge

This project enables an ESP32 microcontroller to interface with a Sony DualSense gaming controller via Bluetooth and remap it's inputs for a different ones.

## Features
- **Bluetooth Connectivity**: ESP32 connects to a DualSense controller to read its inputs.
- **Input Mapping**: Python script translates DualSense inputs to a virtual Xbox360 controller.
- **Web Interface**: ESP32 hosts a web server for remapping DualSense digital buttons.
- **Statistics Logging**: Python script records button press counts in a local SQLite database.
- **Local Operation**: No external network connectivity required for core functionality.

## Hardware Used
- ESP32 development board (e.g., ESP32-WROOM-32)
- Sony DualSense controller
- Computer running the Python script (Windows)

## Software Used
- **PlatformIO**: For programming the ESP32 in C++.
- **ESP32 Libraries**:
  - [ps5-esp32](https://github.com/rodneybakiskan/ps5-esp32) by rodneybabiskan (for DualSense communication)
  - `WiFi` and `ESPAsyncWebServer` (for hosting the web interface)
  - `SPIFFS` (for the web interface files)
- **ViGEmBus Driver**: For creating a virtual Xbox 360 controller.

## Setup Instructions

### Ensure the following are installed:
- [PlatformIO](https://platformio.org/)
- [ViGEmBus Driver](https://vigembusdriver.com/download/)

### 1. Pull the Repository
    ```bash
    git pull https://github.com/jcyran/dualbridge
    ```

### 2. ESP32 Setup
1. Setup PlatformIO in your IDE (e.g., VS Code).
2. Open the `platform-io` folder in you IDE.
3. Configure the `platformio.ini` file with the required ESP32 board and libraries:
    ```ini
    [env:esp32dev]
    platform = espressif32
    board = esp32dev
    framework = arduino
    monitor_speed = 115200
    board_build.partitions = min_spiffs.csv
    lib_deps = 
        esphome/ESPAsyncWebServer-esphome@^3.3.0
    extra_scripts = pre:env_extra.py
    ```
4. Configure the `.env` file
    ```
    WIFI_SSID="ssid"
    WIFI_PASSWORD="password"
    DUALSENSE_MAC="xx:xx:xx:xx:xx:xx"
    ```
5. Upload the firmware to the ESP32 using PlatformIO.

### 4. Running the System
1. Power on the ESP32 and pair it with the DualSense controller via Bluetooth.
2. Navigate to the `prod` directory and read the [Installation Instruction](prod/README.md) to configure and launch the Python script.
3. The script will create a virtual Xbox360 controller.
4. The ESP32 will host a website for mapping the button.
4. After each session, button press statistics are saved to a local SQLite database (`button_stats.db`).

## Usage
- **Button Remapping**: Open the ESP32's web interface (e.g., `http://192.168.x.x`) in a browser to configure button mappings.
- **Virtual Controller**: The Python script emulates an Xbox360 controller, compatible with games or applications that support Xbox360 input.
- **Statistics**: View button press counts in the SQLite database (`button_stats.db`) using any SQLite client (e.g., DB Browser for SQLite).

## Project Structure
- `platform-io/src/main.cpp`: ESP32 C++ code for DualSense connection, WebServer, and button remapping.
- `python/main.py`: Python script for serial communication and virtual controller emulation.
- `python/db.py`: Python script for SQLite logging.

## Notes
- Ensure the ESP32 and computer are on the same local network for web interface access.
- The Python script assumes a serial connection to the ESP32; verify the correct port is used.
- The virtual Xbox360 controller requires [ViGEmBus Driver](https://vigembusdriver.com/download/).

## Troubleshooting
- **Bluetooth Issues**: Ensure the DualSense controller is in pairing mode and no other devices are connected.
- **Serial Connection Errors**: Verify the correct serial port and baud rate in the Python script.
- **Web Server Unreachable**: Check the ESP32's IP address in the serial monitor and ensure the computer is on the same WiFi network.