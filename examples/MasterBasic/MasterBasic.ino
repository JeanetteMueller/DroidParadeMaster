// DroidParadeMaster - Beispiel: setzt alle Parameter und sendet sie alle 20 ms.
#include <DroidParadeMaster.h>

DroidParadeMaster master;

// Muss auf Master und allen Droiden identisch sein. Eigenes Secret wählen und geheim halten!
const DroidParadeConfig paradeConfig = {
  /* paradeId */ 0x0001,
  /* secret   */ "BITTE-AENDERN-geheimes-Passwort",
  /* channel  */ 1
};

void setup() {
  Serial.begin(115200);
  if (!master.begin(paradeConfig)) Serial.println("ESP-NOW Init fehlgeschlagen");
}

void loop() {
  bool blink = (millis() / 500) % 2;
  int  sweep = (int)(255.0f * sin(millis() / 1000.0f));   // -255..+255

  // Body: Nummern sind 1-basiert wie in den Parameternamen
  master.setBodyLeft(1, blink);
  master.setBodyLeft(2, !blink);
  master.setBodyLeft(3, false);
  master.setBodyRight(1, blink);
  master.setBodyRight(2, !blink);
  master.setBodyRight(3, false);
  for (uint8_t i = 1; i <= 12; i++) master.setBodyCenter(i, i == 1 + (millis() / 250) % 12);
  master.setBodyArms(blink);

  // Dome
  master.setDomeRotation(sweep);
  for (uint8_t i = 1; i <= 6; i++) master.setDome(i, i == 1 + (millis() / 400) % 6);

  // Spezialaktionen
  master.setSpecialAction(1, false);
  master.setSpecialAction(2, false);
  master.setSpecialAction(3, false);

  // Licht: DP_LIGHT_OFF, DP_LIGHT_COLOR oder DP_LIGHT_RAINBOW
  master.setLightProp(blink ? DP_LIGHT_COLOR : DP_LIGHT_RAINBOW);
  master.setLightColor(0xFF7800);   // 0xRRGGBB

  master.send();   // am Ende der loop()
  delay(20);
}
