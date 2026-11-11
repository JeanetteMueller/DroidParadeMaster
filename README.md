# DroidParadeMaster

Sender-Bibliothek des **Droid Parade**-Systems. Der Master sammelt alle Steuerdaten und sendet sie per ESP-NOW-Broadcast gleichzeitig an alle Droiden (`DroidParadeSlave`).

**Autorin:** Jeanette Müller · **Version:** 1.0.0 · **Plattform:** ESP32 (Arduino Core 2.x und 3.x) · **Abhängigkeit:** `DroidParadeShared`

## Benutzung
```cpp
#include <DroidParadeMaster.h>
DroidParadeMaster master;

void setup() { master.begin(1); }   // WLAN-Kanal 1; muss mit den Slaves übereinstimmen

void loop() {
  master.setBodyLeft(1, true);      // Nummern 1-basiert
  master.setDomeRotation(120);      // -255..+255
  master.setLightProp(DP_LIGHT_COLOR);
  master.setLightColor(0xFF7800);
  master.send();                    // am Ende der loop()
}
```

## API
`begin(channel)`, `setBodyLeft(n, bool)`, `setBodyRight(n, bool)`, `setBodyCenter(n, bool)`, `setBodyArms(bool)`, `setDomeRotation(int)`, `setDome(n, bool)`, `setSpecialAction(n, bool)`, `setLightProp(prop)`, `setLightColor(uint32_t)`, `clearAll()`, `send()`, `lastSendOk()`.

Das Paket wird per Broadcast an alle Droiden gesendet. Beispiel: `examples/MasterBasic`.
