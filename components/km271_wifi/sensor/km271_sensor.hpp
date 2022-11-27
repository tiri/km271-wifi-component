#pragma once

#include <esphome/components/sensor/sensor.h>

#include "../km271_wifi.hpp"
#include "../buderus_parser.hpp"

namespace esphome {
namespace KM271 {

enum class SensorDecodingOption { NONE, HALFDEGREE, NEGATIVE };

class KM271Sensor : public KM271Listener, public sensor::Sensor {
 public:
  KM271Sensor(uint16_t param_id);
  KM271Sensor(uint16_t param_id, SensorDecodingOption decoding_option);
  void publish_val(BuderusTelegram telegram) override;

 private:
  SensorDecodingOption decoding_option_;
};

}  // namespace KM271
}  // namespace esphome
