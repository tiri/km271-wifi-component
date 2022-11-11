#pragma once

#include <functional>
#include <vector>

#include "esphome/core/log.h"
#include "esphome/core/util.h"

namespace esphome {
namespace KM271 {

#define RXBUFLEN 128
#define lenof(X) (sizeof(X) / sizeof(X[0]))

static const char *const TAG_PROTOCOL = "km271-protocol";

class Protocol3964R {
   private:
    static const char STX = 0x02;
    static const char ETX = 0x03;
    static const char DLE = 0x10;
    static const char NAK = 0x15;

    // Quittierungsverzugszeit: Time-out in milliseconds for achnoledge
    static const uint32_t QVZ = 2000;
    // Zeichenverzugszeit: Time-out between data of active transmission
    static const uint32_t ZVZ = 220;

    enum State3964R {
        WAIT_RX_STX = 0,
        WAIT_RX_DLE_ETX,
        WAIT_RX_CSUM,
        WAIT_TX_ACK_SEND_DATA,
        WAIT_TX_ACK
    };

    uint32_t last_action;
    State3964R state;

   public:
    typedef std::function<void(uint8_t)> WriteCallback;
    WriteCallback writeCallback_;
    typedef std::function<void(std::vector<char>)> TelegramCallback;
    TelegramCallback telegramCallback_;

    Protocol3964R(WriteCallback writeCallback,
                  TelegramCallback telegramCallback)
        : state(WAIT_RX_STX),
          writeCallback_(writeCallback),
          telegramCallback_(telegramCallback) {}

    void process(char c) {
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
            ESP_LOGW(TAG_PROTOCOL, "QVZ time-out");
            state = WAIT_RX_STX;
        }

        csum ^= c;  // Sum up the checksum
        rxLen++;

        switch (state) {
            case WAIT_RX_STX:
                pRxBuf = &rxBuffer[0];
                csum = 0x00;
                rxLen = 0;
                toggleDLE = false;
                if (STX == c) {
                    // ESP_LOGD(TAG_PROTOCOL, "Start Request received, sending
                    // ACK = DLE");
                    this->writeCallback_(DLE);
                    new_state = WAIT_RX_DLE_ETX;
                }
                break;

            case WAIT_RX_DLE_ETX:
                if (timeSinceLA > ZVZ) {
                    ESP_LOGW(TAG_PROTOCOL, "ZVZ time-out");
                    new_state = WAIT_RX_STX;
                    break;
                }
                *pRxBuf = c;

                if (pRxBuf < &rxBuffer[RXBUFLEN]) {
                    // If a double DLE is received, it is DLE = 0x10 data
                    // and not initiating the end of transmission
                    if (ETX == c && toggleDLE) {
                        // ESP_LOGD(TAG_PROTOCOL, "End of Transmission received
                        // -> next CSUM (0x%02X)", csum);
                        savedCsum = csum;
                        new_state = WAIT_RX_CSUM;
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
                    ESP_LOGE(TAG_PROTOCOL, "RX Buffer overrun!");
                    new_state = WAIT_RX_STX;
                }

                break;

            case WAIT_RX_CSUM:
                *pRxBuf = c;
                // ESP_LOGD(TAG_PROTOCOL, "CSUM: S:0x%02X, C:0x%02X, R:0x%02X",
                // savedCsum, csum, c); this->print_hex_buffer(rxBuffer, rxLen);
                if (0x00 == csum && savedCsum == c) {
                    // ESP_LOGI(TAG_PROTOCOL, "Checksum match, sending ACK =
                    // DLE");
                    this->writeCallback_(DLE);
                    // ESP_LOGD(TAG_PROTOCOL, "Received data -> Parsing TODO");
                    //  parse the data
                    std::vector<char> data(rxBuffer, rxBuffer + rxLen - 3);
                    this->telegramCallback_(data);
                } else {
                    ESP_LOGE(TAG_PROTOCOL, "Checksum mismatch, sending NAK");
                    this->writeCallback_(NAK);
                }
                new_state = WAIT_RX_STX;
                break;

            default:
                state = WAIT_RX_STX;
                new_state = WAIT_RX_STX;
                break;
        }

        state = new_state;
        last_action = now;
        lastChar = c;
    }
};

}  // namespace KM271
}  // namespace esphome
