#pragma once

namespace esphome {
namespace KM271 {

struct BuderusTelegram {
  uint16_t id;
  const char *data;
  size_t data_len;
};

class BuderusParser {
 public:
  static BuderusTelegram encode(const char *buffer, size_t buffer_len) {
    if (buffer_len < 3)
      return BuderusTelegram{};
    return BuderusTelegram{static_cast<uint16_t>((buffer[0] << 8) | buffer[1]), buffer + 2, buffer_len - 2};
  }

  // static std::vector<char> decode(BuderusTelegram telegram) {}
};

}  // namespace KM271
}  // namespace esphome
