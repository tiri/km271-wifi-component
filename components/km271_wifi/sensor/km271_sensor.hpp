#pragma once

#include <esphome/components/sensor/sensor.h>

#include "../km271_wifi.hpp"

namespace esphome {
namespace KM271 {

enum class SensorEncodingOption { NONE, HALFDEGREE, NEGATIVE };

class KM271Sensor : public KM271Listener, public sensor::Sensor, public Component {
 public:
  KM271Sensor(uint16_t param_id);
  KM271Sensor(uint16_t param_id, SensorEncodingOption encoding_option);
  void publish_val(const char *buf, size_t len) override;

 private:
  SensorEncodingOption encoding_option_;
};

}  // namespace KM271
}  // namespace esphome
