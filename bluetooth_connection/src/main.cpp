// #include <Arduino.h>
// #include <BluetoothSerial.h>

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

// BluetoothSerial SerialBT;
// uint8_t dualsenseMAC[6] = { 0x24, 0xa6, 0xfa, 0x8d, 0xd3, 0x4c };

// void setup() {
//   // Start the serial monitor
//   Serial.begin(9600);
//   Serial.println("Initializing Bluetooth...");

//   SerialBT.enableSSP();

//   // Initialize Bluetooth
//   if (!SerialBT.begin("ESP32_Device", true)) {
//     Serial.println("An error occurred initializing Bluetooth");
//     return;
//   }

//   SerialBT.setPin("0000");
  
//   Serial.println("Bluetooth initialized successfully. Scanning for devices...");
// }

// void loop() {
//     // Scan for Bluetooth devices
//     Serial.println("Scanning for nearby devices...");

//     // Get the list of paired devices
//     BTScanResults *devices = SerialBT.discover(5000);
//     Serial.printf("Found %d paired devices.\n", devices->getCount());

//     // Print the details of each paired device
//     for (int i = 0; i < devices->getCount(); i++)
//     {
//         BTAdvertisedDevice *device = devices->getDevice(i);
//         Serial.print("Device: ");
//         Serial.print(device->getName().c_str());
//         Serial.print(" Address: ");
//         Serial.println(device->getAddress().toString().c_str());

//         if (device->getName() == "DualSense Wireless Controller")
//         {
//             SerialBT.connect("DualSense Wireless Controller");
//             Serial.println("Connected");
//         }
//     }

//     // Add a delay before scanning again
//     delay(1000); // Wait 5 seconds before rescanning
// }

#include <ps5Controller.h>

void setup() {
  Serial.begin(115200);
  ps5.begin("24:a6:fa:8d:d3:4c"); //replace with MAC address of your controller
  Serial.println("Ready.");
}

void loop() {
//  while (ps5.isConnected() == false) { // commented out as ps5 controller seems to connect quicker when microcontroller is doing nothing
//    Serial.println("PS5 controller not found");
//    delay(300);
//  }

  while (ps5.isConnected() == true) {
    uint8_t buttons, funcButtons;
    //if (ps5.LatestPacket()) Serial.println((char*)(ps5.data.latestPacket));
    /*
    if (ps5.Right()) Serial.println("Right Button");
    if (ps5.Down()) Serial.println("Down Button");
    if (ps5.Up()) Serial.println("Up Button");
    if (ps5.Left()) Serial.println("Left Button");

    if (ps5.Square()) Serial.println("Square Button");
    if (ps5.Cross()) Serial.println("Cross Button");
    if (ps5.Circle()) Serial.println("Circle Button");
    if (ps5.Triangle()) Serial.println("Triangle Button");

    */
    buttons = (ps5.Right() << 7) | (ps5.Down() << 6) | (ps5.Up() << 5) | (ps5.Left() << 4) | (ps5.Square() << 3) | (ps5.Cross() << 2) | (ps5.Circle() << 1) | ps5.Triangle();

    /*
    if (ps5.UpRight()) Serial.println("Up Right");
    if (ps5.DownRight()) Serial.println("Down Right");
    if (ps5.UpLeft()) Serial.println("Up Left");
    if (ps5.DownLeft()) Serial.println("Down Left");

    if (ps5.L1()) Serial.println("L1 Button");
    if (ps5.R1()) Serial.println("R1 Button");

    if (ps5.Share()) Serial.println("Share Button");
    if (ps5.Options()) Serial.println("Options Button");
    if (ps5.L3()) Serial.println("L3 Button");
    if (ps5.R3()) Serial.println("R3 Button");

    if (ps5.PSButton()) Serial.println("PS Button");
    if (ps5.Touchpad()) Serial.println("Touch Pad Button");
    */

    funcButtons = (ps5.L1() << 7) | (ps5.R1() << 6) | (ps5.L3() << 5 ) | (ps5.R3() << 4) | (ps5.Share() << 3) | (ps5.Options() << 2) | (ps5.Touchpad() << 1)| ps5.PSButton();
  /*
    if (ps5.L2()) {
      Serial.printf("L2 button at %d\n", ps5.L2Value());
    }
    if (ps5.R2()) {
      Serial.printf("R2 button at %d\n", ps5.R2Value());
    }

    if (ps5.LStickX()) {
      Serial.printf("Left Stick x at %d\n", ps5.LStickX());
    }
    if (ps5.LStickY()) {
      Serial.printf("Left Stick y at %d\n", ps5.LStickY());
    }
    if (ps5.RStickX()) {
      Serial.printf("Right Stick x at %d\n", ps5.RStickX());
    }
    if (ps5.RStickY()) {
      Serial.printf("Right Stick y at %d\n", ps5.RStickY());
    }
*/

    
    uint64_t message = (buttons << 56) | (funcButtons << 48) | (ps5.L2Value() << 40) | (ps5.R2Value() << 32) | (ps5.LStickX() << 24) | (ps5.LStickY() << 16) | (ps5.RStickX() << 8) | ps5.RStickY();

    char test[8];

    sprintf(test, "%d", buttons);

    Serial.println(message);

    //Serial.println();
    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output
    delay(300);
  }
}
