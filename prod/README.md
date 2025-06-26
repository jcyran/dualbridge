# DualBridge Installation Instructions

## Prerequisites
- Install the ViGEmBus driver: https://github.com/ViGEm/ViGEmBus/releases
  - Download and run `ViGEmBusSetup_x64.exe` (for 64-bit Windows).
  - Reboot if prompted.

## Installation
1. Extract `dualbridge.zip` to a folder (e.g., `C:\DualBridge`).
2. Edit `config.yaml` to customize settings (see below for details).
3. Double-click `dualbridge.exe` to start the app.

## Configuring config.yaml
- The `config.yaml` file is located in the same folder as `dualbridge.exe`.
- Edit it with a text editor (e.g., Notepad) to adjust settings.
- Example:
  ```yaml
  ESP_PORT: "COM3" # Check the COM port in Device Manager
  ESP_BAUDRATE: 115200 # If you didn't change the hardware code leave it as is