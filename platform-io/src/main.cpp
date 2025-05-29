#include <Arduino.h>
#include <ps5Controller.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// WiFi credentials (modify as needed)
const char* ssid = "ssid";
const char* password = "password";

struct DualSenseSensors
{
  uint8_t buttons, funcButtons, L2, R2, LStickX, LStickY, RStickX, RStickY;
  bool rightID, downID, upID, leftID, squareID, crossID, circleID, triangleID,
          l1ID, r1ID, l3ID, r3ID;
  uint8_t* message;
  int messageSize;
  std::vector<bool(ps5Controller::*)()> firstBit, secondBit, thirdBit, fourthBit, fifthBit, sixthBit, seventhBit, eightBit, ninthBit, tenthBit, eleventhBit, twelthBit;

  DualSenseSensors() {
    firstBit.push_back(&ps5Controller::Right);
    secondBit.push_back(&ps5Controller::Down);
    thirdBit.push_back(&ps5Controller::Up);
    fourthBit.push_back(&ps5Controller::Left);
    fifthBit.push_back(&ps5Controller::Square);
    sixthBit.push_back(&ps5Controller::Cross);
    seventhBit.push_back(&ps5Controller::Circle);
    eightBit.push_back(&ps5Controller::Triangle);
    ninthBit.push_back(&ps5Controller::L1);
    tenthBit.push_back(&ps5Controller::R1);
    eleventhBit.push_back(&ps5Controller::L3);
    twelthBit.push_back(&ps5Controller::R3);
  }

  void getMeasurements()
  {
    rightID = 0; downID = 0; upID = 0; leftID = 0;
    squareID = 0; crossID = 0; circleID = 0; triangleID = 0;
    l1ID = 0; r1ID = 0; l3ID = 0; r3ID = 0;

    for (int i = 0; i < firstBit.size(); i++) rightID |= (ps5.*firstBit[i])();
    for (int i = 0; i < secondBit.size(); i++) downID |= (ps5.*secondBit[i])();
    for (int i = 0; i < thirdBit.size(); i++) upID |= (ps5.*thirdBit[i])();
    for (int i = 0; i < fourthBit.size(); i++) leftID |= (ps5.*fourthBit[i])();
    for (int i = 0; i < fifthBit.size(); i++) squareID |= (ps5.*fifthBit[i])();
    for (int i = 0; i < sixthBit.size(); i++) crossID |= (ps5.*sixthBit[i])();
    for (int i = 0; i < seventhBit.size(); i++) circleID |= (ps5.*seventhBit[i])();
    for (int i = 0; i < eightBit.size(); i++) triangleID |= (ps5.*eightBit[i])();
    for (int i = 0; i < ninthBit.size(); i++) l1ID |= (ps5.*ninthBit[i])();
    for (int i = 0; i < tenthBit.size(); i++) r1ID |= (ps5.*tenthBit[i])();
    for (int i = 0; i < eleventhBit.size(); i++) l3ID |= (ps5.*eleventhBit[i])();
    for (int i = 0; i < twelthBit.size(); i++) r3ID |= (ps5.*twelthBit[i])();

    // digital
    buttons = (rightID << 7) | (downID << 6) | (upID << 5) | (leftID << 4) |
              (squareID << 3) | (crossID << 2) | (circleID << 1) | triangleID;

    funcButtons = (l1ID << 7) | (r1ID << 6) | (l3ID << 5) | (r3ID << 4) |
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
AsyncWebServer server(80);

void setupServer();
void changeButton(std::vector<bool(ps5Controller::*)()> &buttonVector, String newButton);

void setup()
{
  Serial.begin(115200);
  SPIFFS.begin(true);
  ps5.begin("24:a6:fa:8d:d3:4c"); // replace with MAC address of your controller

  while (!ps5.isConnected())
    delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(100);

  Serial.println(WiFi.localIP());
  setupServer();

  Serial.println("Ready.");
}

void loop()
{
  while (ps5.isConnected()) {
    dsSensors.getMeasurements();
    dsSensors.getFullMessage();
    dsSensors.sendMessage();

    delay(50);
  }
}

void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

  server.on(
    "/api/buttons",
    HTTP_POST,
    [](AsyncWebServerRequest* request) {},
    NULL,
    [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

    String body = (char*)data;
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);

    // Extract mappings from JSON
    JsonObject mappings = doc["mappings"];

    changeButton(dsSensors.firstBit, mappings["D-Pad_Right"]);
    changeButton(dsSensors.secondBit, mappings["D-Pad_Down"]);
    changeButton(dsSensors.thirdBit, mappings["D-Pad_Up"]);
    changeButton(dsSensors.fourthBit, mappings["D-Pad_Left"]);
    changeButton(dsSensors.fifthBit, mappings["Square"]);
    changeButton(dsSensors.sixthBit, mappings["Cross"]);
    changeButton(dsSensors.seventhBit, mappings["Circle"]);
    changeButton(dsSensors.eightBit, mappings["Triangle"]);
    changeButton(dsSensors.ninthBit, mappings["L1"]);
    changeButton(dsSensors.tenthBit, mappings["R1"]);
    changeButton(dsSensors.eleventhBit, mappings["L3"]);
    changeButton(dsSensors.twelthBit, mappings["R3"]);

    request->send(200, "application/json", "{\"gitara\": \"siema\"}");
  });

  server.begin();
}


void changeButton(std::vector<bool(ps5Controller::*)()> &buttonVector, String newButton){
  buttonVector.clear();
  if( newButton == "Triangle"){
    buttonVector.push_back(&ps5Controller::Triangle);
  }
  else if( newButton == "Circle"){
    buttonVector.push_back(&ps5Controller::Circle);
  }
  else if (newButton == "Cross"){
    buttonVector.push_back(&ps5Controller::Cross);
  }
  else if (newButton == "Square"){
    buttonVector.push_back(&ps5Controller::Square);
  }
  else if (newButton == "D-Pad_Up"){
    buttonVector.push_back(&ps5Controller::Up);
  }
  else if (newButton == "D-Pad_down"){
    buttonVector.push_back(&ps5Controller::Down);
  }
  else if ( newButton == "D-Pad_Left"){
    buttonVector.push_back(&ps5Controller::Left);
  }
  else if (newButton == "D-Pad_Right"){
    buttonVector.push_back(&ps5Controller::Right);
  }
  else if (newButton == "L1"){
    buttonVector.push_back(&ps5Controller::L1);
  }
  else if (newButton == "R1"){
    buttonVector.push_back(&ps5Controller::R1);
  }
  else if( newButton == "L3"){
    buttonVector.push_back(&ps5Controller::L3);
  }
  else if( newButton == "R3"){
    buttonVector.push_back(&ps5Controller::R3);
  }
 
}