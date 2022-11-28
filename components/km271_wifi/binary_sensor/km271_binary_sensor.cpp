#include "km271_binary_sensor.hpp"

#include <esphome/core/log.h>

namespace esphome {
namespace KM271 {
static const char *const TAG = "km271_binary_sensor";

KM271BinarySensor::KM271BinarySensor(uint16_t param_id) : KM271Listener(param_id) {}

void KM271BinarySensor::publish_val(const BuderusTelegram telegram) {
  bool data = telegram.data[0];

  publish_state(data);
}

}  // namespace KM271
}  // namespace esphome
