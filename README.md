# DroidParadeMaster

Sender-Bibliothek des **Droid Parade**-Systems. Der Master sammelt alle Steuerdaten und sendet sie per ESP-NOW-Broadcast gleichzeitig an alle Droiden (`DroidParadeSlave`).

**Autorin:** Jeanette Müller · **Version:** 1.0.0 · **Plattform:** ESP32 (Arduino Core 2.x und 3.x) · **Abhängigkeit:** `DroidParadeShared`

## Benutzung
```cpp
#include <DroidParadeMaster.h>
DroidParadeMaster master;

void setup() { master.begin(paradeConfig); }   // Config siehe unten

void loop() {
  master.setBodyLeft(1, true);      // Nummern 1-basiert
  master.setDomeRotation(120);      // -255..+255
  master.setLightProp(DP_LIGHT_COLOR);
  master.setLightColor(0xFF7800);
  master.send();                    // am Ende der loop()
}
```

## Zugehörigkeit & Sicherheit
Master und Droiden teilen eine `DroidParadeConfig`:

```cpp
const DroidParadeConfig paradeConfig = {
  /* paradeId */ 0x0001,                     // öffentliche Kennung der Parade
  /* secret   */ "mein-geheimes-passwort",   // mind. 8 Zeichen, geheim halten
  /* channel  */ 1                           // WLAN-Kanal
};
```

- Jedes Paket trägt die `paradeId` und ist mit einem gekürzten HMAC-SHA256 über das `secret` signiert.
- Pakete mit anderer Parade-ID, falscher Signatur oder alter Sequenznummer (Replay) werden verworfen (`packetsRejected()`).
- `begin()` liefert `false`, wenn das Secret fehlt oder kürzer als 8 Zeichen ist.
- Die Pakete sind signiert, aber **nicht verschlüsselt**; das Secret darf nicht ins öffentliche Repo.

## API
`begin(config)`, `setBodyLeft(n, bool)`, `setBodyRight(n, bool)`, `setBodyCenter(n, bool)`, `setBodyArms(bool)`, `setDomeRotation(int)`, `setDome(n, bool)`, `setSpecialAction(n, bool)`, `setLightProp(prop)`, `setLightColor(uint32_t)`, `clearAll()`, `send()`, `lastSendOk()`.

Das Paket wird per Broadcast an alle Droiden gesendet. Beispiel: `examples/MasterBasic`.
