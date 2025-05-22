#include <Arduino.h>
#include <ps5Controller.h>

struct DualSenseSensors
{
  uint8_t buttons, funcButtons, L2, R2, LStickX, LStickY, RStickX, RStickY,
          rightID, downID, upID, leftID, squareID, crossID, circleID, triangleID,
          l1ID, r1ID, l3ID, r3ID;
  uint8_t* message;
  int messageSize;

  DualSenseSensors() {
    rightID = 7;
    downID = 6;
    upID = 5;
    leftID = 4;
    squareID = 3;
    crossID = 2;
    circleID = 1;
    triangleID = 0;

    l1ID = 7;
    r1ID = 6;
    l3ID = 5;
    r3ID = 4;
  }

  void getMeasurements()
  {
    // digital
    buttons = (ps5.Right() << rightID) | (ps5.Down() << downID) | (ps5.Up() << upID) | (ps5.Left() << leftID) |
              (ps5.Square() << squareID) | (ps5.Cross() << crossID) | (ps5.Circle() << circleID) | (ps5.Triangle() << triangleID);

    funcButtons = (ps5.L1() << l1ID) | (ps5.R1() << r1ID) | (ps5.L3() << l3ID) | (ps5.R3() << r3ID) |
                  (ps5.Share() << 3) | (ps5.Options() << 2) | (ps5.Touchpad() << 1) | ps5.PSButton();

    // analog
    L2 = ps5.L2Value();
    R2 = ps5.R2Value();
    LStickX = (uint8_t)ps5.LStickX();
    LStickY = (uint8_t)ps5.LStickY();
    RStickX = (uint8_t)ps5.RStickX();
    RStickY = (uint8_t)ps5.RStickY();
  }

  void getFullMessage() {
    message = new uint8_t[8];

    message[0] = buttons;
    message[1] = funcButtons;
    message[2] = L2;
    message[3] = R2;
    message[4] = LStickX;
    message[5] = LStickY;
    message[6] = RStickX;
    message[7] = RStickY;

    messageSize = 8;
  }

  void sendMessage() {
    if (message != nullptr)
      Serial.write(message, messageSize);

    freeMessage();
  }

  void freeMessage() {
    delete [] message;
    messageSize = 0;
  }
};

DualSenseSensors dsSensors;

void setup()
{
  Serial.begin(115200);
  ps5.begin("24:a6:fa:8d:d3:4c"); // replace with MAC address of your controller

  while (!ps5.isConnected())
    delay(100);

  Serial.println("Ready.");
}

void loop()
{
  while (ps5.isConnected()) {
    dsSensors.getMeasurements();
    dsSensors.getFullMessage();
    dsSensors.sendMessage();

    delay(5000);
  }
}
