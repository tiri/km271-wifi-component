#include "km271_wifi.hpp"

#include <esphome/core/log.h>

#include <iomanip>
#include <sstream>

namespace esphome {
namespace KM271 {

static const char *const TAG = "km271";

KM271Listener::KM271Listener(uint16_t id) : id{id} {}

void KM271Component::register_listener(KM271Listener *listener) { listeners_.emplace_back(listener); }

static std::string to_hex(const char *buffer, size_t len) {
  std::ostringstream oss{};

  for (int i = 0; i < len; i++) {
    if (i)
      oss << " ";
    oss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(buffer[i]);
  }

  return oss.str();
}

void KM271Component::recv_telegram_(const std::vector<char> telegram) {
  if (telegram.size() < 2) {
    ESP_LOGE(TAG, "Invalid data length.");
    return;
  }

  const char *input = telegram.data();
  const size_t input_len = telegram.size();

  uint16_t param_id = (input[0] << 8) | input[1];

  const char *data = input + 2;
  size_t data_len = input_len - 2;

  ESP_LOGD(TAG, "Recv telegram 0x%04X: %s", param_id, to_hex(data, data_len).c_str());

  for (auto const &listener : listeners_) {
    if (param_id != listener->id)
      continue;
    listener->publish_val(data, data_len);
  }
}

}  // namespace KM271
}  // namespace esphome
