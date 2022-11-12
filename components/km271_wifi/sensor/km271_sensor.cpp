#include "km271_sensor.hpp"

#include <esphome/core/log.h>

namespace esphome {
namespace KM271 {
static const char *const TAG = "km271_sensor";

KM271Sensor::KM271Sensor(uint16_t param_id) : KM271Sensor(param_id, SensorEncodingOption::NONE) {}

KM271Sensor::KM271Sensor(uint16_t param_id, SensorEncodingOption encoding_option)
    : KM271Listener(param_id), encoding_option_(encoding_option) {}

void KM271Sensor::publish_val(const char *buf, size_t len) {
  uint8_t data = buf[0];

  switch (encoding_option_) {
    case SensorEncodingOption::HALFDEGREE:
      publish_state(((float) data) / 2.0f);
      break;

    case SensorEncodingOption::NEGATIVE:
      if (data > 128) {
        publish_state(((float) (256 - data)) * -1.0f);
      } else {
        publish_state((float) data);
      }

    default:
      publish_state(data);
      break;
  }
}

}  // namespace KM271
}  // namespace esphome
