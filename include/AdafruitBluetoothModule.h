#ifndef ADAFRUITBLUETOOTHMODULE_H
#define ADAFRUITBLUETOOTHMODULE_H

#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#include "PacketParserConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define MODE_LED_BEHAVIOUR          "MODE"

// extern uint8_t packetBuffer[];

namespace Bluetooth
{
    struct Button
    {
        int buttonID;
        int buttonState;
    };
    class AdafruitModule
    {
        private:
            Adafruit_BluefruitLE_SPI* bleModule;
            void initializeModule();
            Bluetooth::Button* button;
        public:
        AdafruitModule();
        ~AdafruitModule();
        Button* getControllerButtons();
    };
}

namespace packetParser
{
    struct Packet
    {
        uint8_t replyIdx;
        uint8_t* packetBuffer;
    };
    float parsefloat(uint8_t *buffer);
    void printHex(const uint8_t * data, const uint32_t numBytes);
    uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout, packetParser::Packet& packet);
}

#endif 