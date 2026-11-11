/*
 * DroidParadeMaster - sendet das gemeinsame Paket per ESP-NOW (Broadcast)
 * Autorin: Jeanette Müller
 */
#pragma once
#include <Arduino.h>
#include <DroidParadeShared.h>

class DroidParadeMaster {
public:
  // Initialisiert WiFi (STA) und ESP-NOW. false bei leerem/zu kurzem Secret oder Fehler.
  bool begin(const DroidParadeConfig &config);

  // Setter (Nummern 1-basiert wie in den Parameternamen)
  void setBodyLeft(uint8_t n, bool on)    { _p.bodyLeft = withBit(_p.bodyLeft, n, DP_BODY_LEFT_COUNT, on); }
  void setBodyRight(uint8_t n, bool on)   { _p.bodyRight = withBit(_p.bodyRight, n, DP_BODY_RIGHT_COUNT, on); }
  void setBodyCenter(uint8_t n, bool on)  { _p.bodyCenter = withBit(_p.bodyCenter, n, DP_BODY_CENTER_COUNT, on); }
  void setBodyArms(bool on)               { _p.bodyArms = on; }
  void setDomeRotation(int16_t v)         { _p.domeRotation = constrain(v, -DP_DOME_ROTATION_MAX, DP_DOME_ROTATION_MAX); }
  void setDome(uint8_t n, bool on)        { _p.dome = withBit(_p.dome, n, DP_DOME_COUNT, on); }
  void setSpecialAction(uint8_t n, bool on) { _p.specialAction = withBit(_p.specialAction, n, DP_SPECIAL_COUNT, on); }
  void setLightProp(DroidParadeLightProp m) { _p.lightProp = m; }
  void setLightColor(uint32_t c)          { _p.lightColor = c; }
  void clearAll()                         { memset(&_p, 0, sizeof(_p)); }

  // Am Ende der loop() aufrufen. true, wenn Senden angestoßen wurde.
  bool send();

  const DroidParadePacket &packet() const { return _p; }
  bool lastSendOk() const                 { return _lastOk; }
  static void _notify(bool ok)            { _lastOk = ok; }  // intern (Callback)

private:
  static uint32_t withBit(uint32_t mask, uint8_t n, uint8_t count, bool on) {
    if (n < 1 || n > count) return mask;
    return on ? (mask | (1UL << (n - 1))) : (mask & ~(1UL << (n - 1)));
  }
  DroidParadePacket _p = {};
  uint32_t _seq = 0;
  const char *_secret = nullptr;
  uint16_t _paradeId = 0;
  bool _ready = false;
  static volatile bool _lastOk;
};
