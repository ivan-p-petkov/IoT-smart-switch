#define BLYNK_TEMPLATE_ID "....................."
#define BLYNK_DEVICE_NAME "....................."

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = ".........................";
char ssid[] = ".........................";
char pass[] = ".........................";

#define relayPin D2
//#define secondRelayPin D3
#define pushButtonPin D1

int relayState = LOW;  // Initially set to LOW (off)
int pushButtonState = HIGH;
int previousPushButtonState = HIGH;

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, !relayState);  // Invert the logic

//  pinMode(secondRelayPin, OUTPUT);
//  digitalWrite(secondRelayPin, !relayState);  // Invert the logic

  pinMode(pushButtonPin, INPUT_PULLUP);
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt();
  if (buttonState == 1) {
    relayState = HIGH;  // Turn on the relays
  } else {
    relayState = LOW;  // Turn off the relays
  }
  
  digitalWrite(relayPin, !relayState);  // Invert the logic
//  digitalWrite(secondRelayPin, !relayState);  // Invert the logic
  
  Blynk.virtualWrite(V1, relayState);  // Update relay state in Blynk app
}

void checkPushButton() {
  pushButtonState = digitalRead(pushButtonPin);
  if (pushButtonState != previousPushButtonState) {
    delay(50);
    pushButtonState = digitalRead(pushButtonPin);
    if (pushButtonState != previousPushButtonState) {
      if (pushButtonState == LOW) {
        relayState = !relayState;
        digitalWrite(relayPin, !relayState);  // Invert the logic
//        digitalWrite(secondRelayPin, !relayState);  // Invert the logic
      }
      previousPushButtonState = pushButtonState;
    }
  }
  
  Blynk.virtualWrite(V1, relayState);  // Update relay state in Blynk app
}

void loop() {
  Blynk.run();
  timer.run();

  checkPushButton();
}
