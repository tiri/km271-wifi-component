#pragma once

// #include <esphome/components/sensor/sensor.h>

#include <esphome/components/uart/uart.h>
#include <esphome/core/component.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "protocol_3964r.hpp"

namespace esphome {
namespace KM271 {

class KM271Component : public Component, public uart::UARTDevice {
   private:
    const int ALIVE_RST = 20000;
    Protocol3964R protocol_;

    void recv_telegram(std::vector<char> telegram);
    void parse_buderus(const char* buf, size_t len);
    size_t gen_data_string(char* outbuf, const char* inbuf, size_t len);

   public:
    KM271Component();
    void loop() override;
    float get_setup_priority() const override;
};

}  // namespace KM271
}  // namespace esphome