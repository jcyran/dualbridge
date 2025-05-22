#include <Arduino.h>
#include <ps5Controller.h>

struct DualSenseSensors
{
  uint8_t buttons, funcButtons, L2, R2, LStickX, LStickY, RStickX, RStickY;
  uint8_t buttonsID, funcButtonsID, L2ID, R2ID, LStickXID, LStickYID, RStickXID, RStickYID;
  uint8_t* message;
  int messageSize;

  DualSenseSensors() {
    buttonsID = 0;
    funcButtonsID = 1;
    L2ID = 2;
    R2ID = 3;
    LStickXID = 4;
    LStickYID = 5;
    RStickXID = 6;
    RStickYID = 7;
  }

  void getMeasurements()
  {
    buttons = (ps5.Right() << 7) | (ps5.Down() << 6) | (ps5.Up() << 5) | (ps5.Left() << 4) | (ps5.Square() << 3) | (ps5.Cross() << 2) | (ps5.Circle() << 1) | ps5.Triangle();
    funcButtons = (ps5.L1() << 7) | (ps5.R1() << 6) | (ps5.L3() << 5) | (ps5.R3() << 4) | (ps5.Share() << 3) | (ps5.Options() << 2) | (ps5.Touchpad() << 1) | ps5.PSButton();
    L2 = ps5.L2Value();
    R2 = ps5.R2Value();
    LStickX = (uint8_t)ps5.LStickX();
    LStickY = (uint8_t)ps5.LStickY();
    RStickX = (uint8_t)ps5.RStickX();
    RStickY = (uint8_t)ps5.RStickY();
  }

  void getFullMessage() {
    message = new uint8_t[8];

    // digital
    message[buttonsID] = buttons;
    message[funcButtonsID] = funcButtons;

    // analog
    message[L2ID] = L2;
    message[R2ID] = R2;
    message[LStickXID] = LStickX;
    message[LStickYID] = LStickY;
    message[RStickXID] = RStickX;
    message[RStickYID] = RStickY;

    messageSize = 8;
  }

  void sendMessage() {
    if (message != nullptr)
      Serial.write(message, messageSize);
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
  while (ps5.isConnected())
  {
    dsSensors.getMeasurements();
    dsSensors.getFullMessage();
    dsSensors.sendMessage();

    delay(5000);
  }
}
