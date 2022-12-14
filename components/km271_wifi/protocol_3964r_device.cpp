#include "protocol_3964r_device.hpp"

#include <esphome/core/log.h>

#include "util.hpp"

namespace esphome {
namespace KM271 {

static const char *const TAG = "3964R";

Parser3964R::Parser3964R() : state(WAITING_FOR_START), current_telegram_length(0), has_unhandled_dle_(false) {}

void Parser3964R::reset() {
  state = WAITING_FOR_START;
  current_telegram_length = 0;
  has_unhandled_dle_ = false;
  current_checksum_ = 0;
}

void Parser3964R::start_telegram() {
  current_telegram_length = 0;
  has_unhandled_dle_ = false;
  state = WAITING_FOR_ETX;
  current_checksum_ = 0;
}

void Parser3964R::consume_byte(uint8_t byte) {
  if (state == WAITING_FOR_ETX) {
    if (current_telegram_length >= MAX_TELEGRAM_SIZE) {
      ESP_LOGE(TAG, "RX Buffer overrun!");
      state = PARSE_ERROR;
      return;
    }

    current_checksum_ ^= byte;

    if (byte == DLE) {
      if (!has_unhandled_dle_) {
        has_unhandled_dle_ = true;
        return;
      } else {
        has_unhandled_dle_ = false;
      }
    } else if (byte == ETX) {
      if (has_unhandled_dle_) {
        state = WAITING_FOR_CHECKSUM;
        has_unhandled_dle_ = false;
        return;
      }
    } else {
      if (has_unhandled_dle_) {
        // log error
        state = PARSE_ERROR;
      }
    }

    telegram[current_telegram_length] = byte;
    current_telegram_length += 1;
  } else if (state == WAITING_FOR_CHECKSUM) {
    if (byte == current_checksum_) {
      // good
      state = TELEGRAM_COMPLETE;
    } else {
      state = PARSE_ERROR;
      // bad
    }

  } else {
    // log error: invalid state
    return;
  }
}

bool Parser3964R::parsing_in_progress() { return state == WAITING_FOR_ETX || state == WAITING_FOR_CHECKSUM; }

Writer3964R::Writer3964R() : writer_state(IDLE), retry_count(0) {}

void Writer3964R::reset() {
  has_unhandled_dle_ = false;
  bytes_sent_ = 0;
  telegram_length_ = 0;
  writer_state = IDLE;
  retry_count = 0;
}

void Writer3964R::enqueue_telegram(uint8_t *data, uint16_t length) {
  if (writer_state != IDLE) {
    ESP_LOGE(TAG, "Writer not idle");
    return;
  }
  if (length > MAX_TELEGRAM_SIZE) {
    ESP_LOGE(TAG, "Telegram too long %d", length);
    return;
  }

  has_unhandled_dle_ = false;
  bytes_sent_ = 0;

  memcpy(telegram_to_send_, data, length);
  telegram_length_ = length;
  writer_state = REQUEST_PENDING;
  retry_count = 0;
}

void Writer3964R::set_stx_sent() {
  writer_state = WAITING_FOR_DLE;
  ESP_LOGD(TAG, "stx sent, waiting for dle");
}

uint8_t Writer3964R::pop_next_byte() {
  if (writer_state == IDLE || writer_state == REQUEST_PENDING) {
    ESP_LOGE(TAG, "Invalid state for pop: %d", writer_state);
    return 0;
  }
  if (writer_state == WAITING_FOR_DLE) {
    // ESP_LOGD(TAG, "Starting to send");
    current_checksum_ = 0;
    writer_state = SENDING;
  }

  if (writer_state == SENDING) {
    if (bytes_sent_ >= telegram_length_) {
      ESP_LOGE(TAG, "too many bytes sent");
      return 0;
    }

    uint8_t c = telegram_to_send_[bytes_sent_];
    if (c == DLE) {
      if (has_unhandled_dle_) {
        has_unhandled_dle_ = false;
      } else {
        has_unhandled_dle_ = true;
        ESP_LOGD(TAG, "Sending escaped DLE");
        current_checksum_ ^= DLE;
        return DLE;
      }
    }
    bytes_sent_++;
    if (bytes_sent_ >= telegram_length_) {
      writer_state = SENDING_DLE;
    }
    current_checksum_ ^= c;
    // ESP_LOGD(TAG, "Sb %02X.", c);
    return c;
  } else if (writer_state == SENDING_DLE) {
    // ESP_LOGD(TAG, "S DLE.");
    current_checksum_ ^= DLE;
    writer_state = SENDING_ETX;
    return DLE;
  } else if (writer_state == SENDING_ETX) {
    // ESP_LOGD(TAG, "S ETX.");
    writer_state = SENDING_CHECKSUM;
    current_checksum_ ^= ETX;
    return ETX;
  } else if (writer_state == SENDING_CHECKSUM) {
    writer_state = WAIT_FOR_ACK;
    // ESP_LOGD(TAG, "SCS %02X", currentChecksum);
    return current_checksum_;
  }

  ESP_LOGE(TAG, "Invalid state %d", writer_state);
  return 0;
}

bool Writer3964R::has_byte_to_send() {
  return writer_state == WAITING_FOR_DLE || writer_state == SENDING || writer_state == SENDING_DLE ||
         writer_state == SENDING_ETX || writer_state == SENDING_CHECKSUM;
}

void Writer3964R::restart_telegram() {
  if (writer_state == WAIT_FOR_ACK || writer_state == WAITING_FOR_DLE || writer_state == REQUEST_PENDING) {
    writer_state = REQUEST_PENDING;
    bytes_sent_ = 0;
    current_checksum_ = 0;
    retry_count++;
  } else {
    ESP_LOGE(TAG, "Can't restart: invalid state %d", writer_state);
  }
}

void P3964RDevice::loop() {
  while (available()) {
    uint8_t c = read();

    // if we have a write, start our request on a stx from the km217. This seems more reliable.
    if (c == STX && parser_.state == WAITING_FOR_START && writer_.writer_state == REQUEST_PENDING) {
      write_byte(STX);
      writer_.set_stx_sent();
    } else {
      process_incoming_byte_(c);
    }
  };
}

float P3964RDevice::get_setup_priority() const { return setup_priority::DATA; }

void P3964RDevice::process_incoming_byte_(uint8_t data) {
  const uint32_t now = millis();

  if (writer_.writer_state == WAITING_FOR_DLE) {
    if (data == DLE) {
      while (writer_.has_byte_to_send()) {
        uint8_t byte = writer_.pop_next_byte();
        write_byte(byte);
      }
      return;
    } else {
      ESP_LOGW(TAG, "no dle received: %2X", data);
      writer_.restart_telegram();
    }
  } else if (writer_.writer_state == WAIT_FOR_ACK) {
    if (data == DLE) {
      writer_.reset();
      ESP_LOGD(TAG, "ack received");
    } else if (data == NAK) {
      if (writer_.retry_count < MAX_TELEGRAM_RETRIES) {
        writer_.restart_telegram();
        ESP_LOGW(TAG, "nack received, retrying");
      } else {
        writer_.reset();
        ESP_LOGE(TAG, "nack received and retry count exhausted, aborting");
      }
    } else {
      if (writer_.retry_count < MAX_TELEGRAM_RETRIES) {
        writer_.restart_telegram();
        ESP_LOGW(TAG, "ack for writer was invalid, retrying: %d", data);
      } else {
        writer_.reset();
        ESP_LOGE(TAG, "ack for writer was invalid and retry count exhausted, aborting: %d", data);
      }
    }
    return;
  }
  uint32_t time_since_la = now - last_received_byte_time_;

  if (time_since_la > ZVZ && parser_.parsing_in_progress()) {
    // Reset transaction when QVZ is over
    ESP_LOGW(TAG, "ZVZ time-out, recv: %2X, state %d", data, parser_.state);
    parser_.reset();
  }
  last_received_byte_time_ = now;

  if (parser_.parsing_in_progress()) {
    parser_.consume_byte(data);

    if (parser_.state == TELEGRAM_COMPLETE) {
      write_byte(DLE);
      recv_telegram(parser_.telegram, parser_.current_telegram_length);
      parser_.reset();
    }
  } else {
    if (data == STX) {
      // ESP_LOGD(TAG, "Start Request received, sending ACK = DLE");
      write_byte(DLE);
      parser_.start_telegram();
    }
  }
}

void P3964RDevice::send_telegram(uint8_t *data, size_t data_len) {
  ESP_LOGD(TAG, "Send telegram: %s", to_hex(data, data_len).c_str());
  writer_.enqueue_telegram(data, data_len);
}

}  // namespace KM271
}  // namespace esphome
