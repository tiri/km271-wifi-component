#pragma once

#include <iomanip>
#include <sstream>

namespace esphome {
namespace KM271 {

static std::string to_hex(const char *buffer, size_t len) {
  std::ostringstream oss{};

  for (int i = 0; i < len; i++) {
    if (i)
      oss << " ";
    oss << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(buffer[i]);
  }

  return oss.str();
}

}  // namespace KM271
}  // namespace esphome
