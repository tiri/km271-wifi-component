#pragma once

#include "protocol_3964r_device.hpp"
#include "buderus_parser.hpp"

namespace esphome {
namespace KM271 {

class KM271Listener {
 public:
  uint16_t id;
  KM271Listener(uint16_t id);
  virtual void publish_val(BuderusTelegram telegram) = 0;
};

class KM271Component : public P3964RDevice {
 public:
  void register_listener(KM271Listener *listener);

 private:
  std::vector<KM271Listener *> listeners_{};
  void recv_telegram_(const char *data, size_t data_len) override;
};

}  // namespace KM271
}  // namespace esphome
