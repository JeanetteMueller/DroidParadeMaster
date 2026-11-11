#include "DroidParadeMaster.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

volatile bool DroidParadeMaster::_lastOk = false;

// Callback-Signatur wechselt ab ESP32 Arduino Core 3.x
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
static void onSent(const wifi_tx_info_t *, esp_now_send_status_t s) {
#else
static void onSent(const uint8_t *, esp_now_send_status_t s) {
#endif
  DroidParadeMaster::_notify(s == ESP_NOW_SEND_SUCCESS);
}

bool DroidParadeMaster::begin(const DroidParadeConfig &config) {
  if (!config.secret || strlen(config.secret) < 8) return false;
  uint8_t channel = config.channel;
  _secret = config.secret;
  _paradeId = config.paradeId;
  _seq = esp_random();  // zufälliger Start: Neustart wird vom Slave akzeptiert
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  if (esp_now_init() != ESP_OK) return false;
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memset(peer.peer_addr, 0xFF, 6);  // Broadcast an alle Slaves
  peer.channel = channel;
  peer.encrypt = false;
  if (!esp_now_is_peer_exist(peer.peer_addr) && esp_now_add_peer(&peer) != ESP_OK) return false;

  _ready = true;
  return true;
}

bool DroidParadeMaster::send() {
  if (!_ready) return false;
  _p.paradeId = _paradeId;
  _p.sequence = ++_seq;
  droidParadeSeal(_p, _secret);
  static const uint8_t bcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  return esp_now_send(bcast, (const uint8_t *)&_p, sizeof(_p)) == ESP_OK;
}
