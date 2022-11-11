#include "km271.hpp"

// #include <esphome/components/sensor/sensor.h>
#include <esphome/core/log.h>
#include <esphome/core/util.h>

#include <functional>
#include <iomanip>
#include <sstream>

#include "protocol_3964r.hpp"
#include "protocol_buderus_2017.hpp"

namespace esphome {
namespace KM271 {

static const char *const TAG = "km271";

KM271Component::KM271Component()
    : protocol_{
          std::bind(&KM271Component::write_byte, this, std::placeholders::_1),
          std::bind(&KM271Component::recv_telegram, this,
                    std::placeholders::_1)} {}

void KM271Component::loop() {
    while (available()) {
        protocol_.process(read());
    };
}

float KM271Component::get_setup_priority() const {
    return setup_priority::DATA;
}

static std::string to_hex(const char *buffer, size_t len) {
    std::ostringstream oss{};

    for (int i = 0; i < len; i++) {
        if (i) oss << " ";
        oss << "0x" << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(buffer[i]);
    }

    return oss.str();
}

void KM271Component::recv_telegram(std::vector<char> telegram) {
    /*ESP_LOGD(TAG, "recv telegram len: %d, data: %s", telegram.size(),
             to_hex(telegram.data(), telegram.size()).c_str());*/

    parse_buderus(telegram.data(), telegram.size());
}

void KM271Component::parse_buderus(const char *input, size_t input_len) {
    if (input_len < 2) {
        ESP_LOGE(TAG, "Invalid data length.");
        return;
    }

    uint16_t id = (input[0] << 8) | input[1];

    if (buderus2017Params.find(static_cast<Buderus2017Ids>(id)) ==
        buderus2017Params.cend()) {
        return;
    }
    const auto desc = buderus2017Params.at(static_cast<Buderus2017Ids>(id));

    const char *data = input + 2;
    size_t data_len = input_len - 2;

    ESP_LOGD(TAG, "Found param 0x%04X: %s %s (Data: %s, %d)", id, desc.name,
             desc.unit, to_hex(data, data_len).c_str(), data[0]);
}

}  // namespace KM271
}  // namespace esphome