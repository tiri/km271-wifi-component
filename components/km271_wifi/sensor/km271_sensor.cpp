#include "km271_sensor.hpp"

#include <esphome/core/log.h>

namespace esphome {
namespace KM271 {
static const char *const TAG = "km271_sensor";

KM271Sensor::KM271Sensor(uint16_t param_id) : KM271Sensor(param_id, SensorDecodingOption::NONE) {}

KM271Sensor::KM271Sensor(uint16_t param_id, SensorDecodingOption decoding_option)
    : KM271Listener(param_id), decoding_option_(decoding_option) {}

void KM271Sensor::publish_val(const BuderusTelegram telegram) {
  uint8_t data = telegram.data[0];

  switch (decoding_option_) {
    case SensorDecodingOption::HALFDEGREE:
      publish_state(((float) data) / 2.0f);
      break;

    case SensorDecodingOption::NEGATIVE:
      if (data > 128) {
        publish_state(((float) (256 - data)) * -1.0f);
      } else {
        publish_state((float) data);
      }
      break;
      
    default:
      publish_state(data);
      break;
  }
}

}  // namespace KM271
}  // namespace esphome
