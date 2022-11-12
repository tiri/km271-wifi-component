#pragma once

#include <vector>

#include "protocol_3964r_device.hpp"

namespace esphome {
namespace KM271 {

class KM271Listener {
 public:
  uint16_t id;
  KM271Listener(uint16_t id);
  virtual void publish_val(const char *buf, std::size_t len) = 0;
};

class KM271Component : public P3964RDevice {
 public:
  void register_listener(KM271Listener *listener);

 private:
  std::vector<KM271Listener *> listeners_{};
  void recv_telegram_(std::vector<char> telegram) override;
};

}  // namespace KM271
}  // namespace esphome
