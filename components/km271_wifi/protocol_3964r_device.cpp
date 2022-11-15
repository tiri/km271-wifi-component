#include "protocol_3964r_device.hpp"

#include <esphome/core/log.h>

namespace esphome {
namespace KM271 {

#define RXBUFLEN 128
#define lenof(X) (sizeof(X) / sizeof(X[0]))

static const char STX = 0x02;
static const char ETX = 0x03;
static const char DLE = 0x10;
static const char NAK = 0x15;

// Quittierungsverzugszeit: Time-out in milliseconds for achnoledge
static const uint32_t QVZ = 2000;
// Zeichenverzugszeit: Time-out between data of active transmission
static const uint32_t ZVZ = 220;

static const char *const TAG = "3964R";

void P3964RDevice::loop() {
  while (available()) {
    handle_rx_(read());
  };
}

float P3964RDevice::get_setup_priority() const { return setup_priority::DATA; }

void P3964RDevice::handle_rx_(char c) {
  static char lastChar = 0x00;
  static char csum, savedCsum;
  static char rxBuffer[RXBUFLEN];
  static int rxLen = 0;
  static char *pRxBuf;
  static bool toggleDLE;

  State3964R new_state = state;

  const uint32_t now = millis();
  uint32_t timeSinceLA = now - last_action;

  if (timeSinceLA > QVZ) {
    // Reset transaction when QVZ is over
    ESP_LOGW(TAG, "QVZ time-out");
    state = State3964R::WAIT_RX_STX;
  }

  csum ^= c;  // Sum up the checksum
  rxLen++;

  switch (state) {
    case State3964R::WAIT_RX_STX:
      pRxBuf = &rxBuffer[0];
      csum = 0x00;
      rxLen = 0;
      toggleDLE = false;
      if (STX == c) {
        // ESP_LOGD(TAG, "Start Request received, sending
        // ACK = DLE");
        write_byte(DLE);
        new_state = State3964R::WAIT_RX_DLE_ETX;
      }
      break;

    case State3964R::WAIT_RX_DLE_ETX:
      if (timeSinceLA > ZVZ) {
        ESP_LOGW(TAG, "ZVZ time-out");
        new_state = State3964R::WAIT_RX_STX;
        break;
      }
      *pRxBuf = c;

      if (pRxBuf < &rxBuffer[RXBUFLEN]) {
        // If a double DLE is received, it is DLE = 0x10 data
        // and not initiating the end of transmission
        if (ETX == c && toggleDLE) {
          // ESP_LOGD(TAG, "End of Transmission received
          // -> next CSUM (0x%02X)", csum);
          savedCsum = csum;
          new_state = State3964R::WAIT_RX_CSUM;
          pRxBuf++;
        } else if (DLE == c && toggleDLE) {
          // We have a 0x10 byte of data (not control char)
          // don't move the buffer pointer (we wrote 0x10 a cycle
          // before)
        } else {
          pRxBuf++;
        }

        // Recognition of two 0x10 as data or single 0x10 as control
        if (DLE == c) {
          toggleDLE = !(toggleDLE);
        } else {
          toggleDLE = false;
        }

      } else {
        ESP_LOGE(TAG, "RX Buffer overrun!");
        new_state = State3964R::WAIT_RX_STX;
      }

      break;

    case State3964R::WAIT_RX_CSUM:
      *pRxBuf = c;
      // ESP_LOGD(TAG, "CSUM: S:0x%02X, C:0x%02X, R:0x%02X",
      // savedCsum, csum, c); this->print_hex_buffer(rxBuffer, rxLen);
      if (0x00 == csum && savedCsum == c) {
        // ESP_LOGI(TAG, "Checksum match, sending ACK = DLE");
        write_byte(DLE);
        //  parse the data
        recv_telegram_(rxBuffer, rxLen - 3);
      } else {
        ESP_LOGE(TAG, "Checksum mismatch, sending NAK");
        write_byte(NAK);
      }
      new_state = State3964R::WAIT_RX_STX;
      break;

    default:
      state = State3964R::WAIT_RX_STX;
      new_state = State3964R::WAIT_RX_STX;
      break;
  }

  state = new_state;
  last_action = now;
  lastChar = c;
}

void P3964RDevice::send_telegram_(const char* data, size_t data_len) {}
}  // namespace KM271
}  // namespace esphome
