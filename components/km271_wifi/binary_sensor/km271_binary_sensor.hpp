#pragma once

#include <esphome/components/binary_sensor/binary_sensor.h>

#include "../km271_wifi.hpp"
#include "../buderus_parser.hpp"

namespace esphome {
namespace KM271 {

class KM271BinarySensor : public KM271Listener, public binary_sensor::BinarySensor {
 public:
  KM271BinarySensor(uint16_t param_id);
  void publish_val(BuderusTelegram telegram) override;
};

}  // namespace KM271
}  // namespace esphome
