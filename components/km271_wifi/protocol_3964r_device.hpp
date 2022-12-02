#pragma once

#include <esphome/components/uart/uart.h>
#include <esphome/core/component.h>

#define MAX_TELEGRAM_SIZE 128

namespace esphome {
namespace KM271 {

static const char STX = 0x02;
static const char ETX = 0x03;
static const char DLE = 0x10;
static const char NAK = 0x15;

static const uint32_t QVZ = 2000;  // Quittierungsverzugszeit: Time-out in milliseconds for achnowledge
static const uint32_t ZVZ = 220;   // Zeichenverzugszeit: Time-out between data of active transmission

static const uint8_t MAX_TELEGRAM_RETRIES = 5;  // stop if no ack after this amount of tries

enum ParserState { 
  WAITING_FOR_START,
  WAITING_FOR_ETX,
  WAITING_FOR_CHECKSUM,
  TELEGRAM_COMPLETE,
  PARSE_ERROR
};

class Parser3964R {
 public:
  Parser3964R();

  void reset();
  void start_telegram();
  void consume_byte(uint8_t byte);
  bool parsing_in_progress();

  uint8_t telegram[MAX_TELEGRAM_SIZE];
  ParserState state;
  uint16_t current_telegram_length;

 private:
  bool has_unhandled_dle_;
  uint8_t current_checksum_;
};

enum WriterState {
  IDLE,
  REQUEST_PENDING,
  WAITING_FOR_DLE,
  SENDING,
  SENDING_DLE,
  SENDING_ETX,
  SENDING_CHECKSUM,
  WAIT_FOR_ACK
};

class Writer3964R {
 public:
  Writer3964R();
  void reset();
  void enqueue_telegram(uint8_t *data, uint16_t length);
  void set_stx_sent();
  uint8_t pop_next_byte();
  bool has_byte_to_send();
  void restart_telegram();

  WriterState writer_state;
  uint8_t retry_count;

 private:
  uint8_t telegram_to_send_[MAX_TELEGRAM_SIZE];
  uint16_t telegram_length_;
  bool has_unhandled_dle_;
  uint8_t current_checksum_;
  uint16_t bytes_sent_;
};

class P3964RDevice : public Component, public uart::UARTDevice {
 public:
  void loop() override;
  float get_setup_priority() const override;

 protected:
  virtual void recv_telegram(uint8_t *data, size_t data_len) = 0;
  void send_telegram(uint8_t *data, size_t data_len);

 private:
  Parser3964R parser_;
  Writer3964R writer_;
  void process_incoming_byte_(uint8_t data);
  uint32_t last_received_byte_time_;
};

}  // namespace KM271
}  // namespace esphome
