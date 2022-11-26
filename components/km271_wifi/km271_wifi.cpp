#include "km271_wifi.hpp"

#include <esphome/core/log.h>

#include "util.hpp"

namespace esphome {
namespace KM271 {

static const char *const TAG = "km271";

KM271Listener::KM271Listener(uint16_t id) : id{id} {}

void KM271Component::register_listener(KM271Listener *listener) { listeners_.emplace_back(listener); }

void KM271Component::recv_telegram_(uint8_t *data, size_t data_len) {
  if (data_len < 2) {
    ESP_LOGE(TAG, "Invalid data length.");
    return;
  }

  const BuderusTelegram telegram = BuderusParser::encode(data, data_len);

  ESP_LOGD(TAG, "Recv telegram 0x%04X: %s", telegram.id, to_hex(telegram.data, telegram.data_len).c_str());

  for (auto const &listener : listeners_) {
    if (telegram.id != listener->id)
      continue;
    listener->publish_val(telegram);
  }
}

void KM271Component::setup() {
  uint8_t logCommand[] = {0xEE, 0x00, 0x00};
  send_telegram_(logCommand, 3);
};

}  // namespace KM271
}  // namespace esphome
