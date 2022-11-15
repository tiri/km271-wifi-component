#pragma once

#include <esphome/components/uart/uart.h>
#include <esphome/core/component.h>

namespace esphome {
namespace KM271 {

class P3964RDevice : public Component, public uart::UARTDevice {
 public:
  void loop() override;
  float get_setup_priority() const override;

 protected:
  virtual void recv_telegram_(const char *data, size_t data_len) = 0;
  void send_telegram_(const char *data, size_t data_len);

 private:
  void handle_rx_(char in);
  enum class State3964R { WAIT_RX_STX = 0, WAIT_RX_DLE_ETX, WAIT_RX_CSUM, WAIT_TX_ACK_SEND_DATA, WAIT_TX_ACK };

  uint32_t last_action;
  State3964R state;
};
}  // namespace KM271
}  // namespace esphome
