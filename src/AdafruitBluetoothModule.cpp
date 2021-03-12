#include "AdafruitBluetoothModule.h"

uint8_t packetBuffer[READ_BUFSIZE+1];

Bluetooth::AdafruitModule::AdafruitModule() : bleModule(new Adafruit_BluefruitLE_SPI(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST))
{
    button = new Button();
    initializeModule();
}
Bluetooth::AdafruitModule::~AdafruitModule()
{
    delete bleModule;
    delete button;
}

void Bluetooth::AdafruitModule::initializeModule()
{
    delay(10);

    if (!Serial)
    {
        Serial.begin(9600);
    }

    /* Initialise the module */
#ifdef DEBUG
    Serial.print(F("Initialising the Bluefruit LE module: "));
#endif

#ifdef DEBUG
    if (!bleModule->begin(true))
    {
        Serial.print("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
    }
#endif
#ifndef DEBUG
    bleModule->begin(false);
#endif

#ifdef DEBUG
    Serial.println(F("OK!"));
#endif

    if (FACTORYRESET_ENABLE)
    {
/* Perform a factory reset to make sure everything is in a known state */
#ifdef DEBUG
        Serial.println(F("Performing a factory reset: "));
#endif
        if (!bleModule->factoryReset())
        {
#ifdef DEBUG
            Serial.print(F("Couldn't factory reset"));
#endif
        }
    }

    /* Disable command echo from Bluefruit */
    bleModule->echo(false);

#ifdef DEBUG
    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    bleModule->info();

    Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
    Serial.println();
#endif

    bleModule->verbose(false); // debug info is a little annoying after this point!

    /* Wait for connection */
    while (!bleModule->isConnected())
    {
        delay(500);
    }

#ifdef DEBUG
    Serial.println(F("******************************"));
#endif

    // LED Activity command is only supported from 0.6.6
    if (bleModule->isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
    {
        // Change Mode LED Activity
#ifdef DEBUG
        Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
#endif
        bleModule->sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    }

    // Set Bluefruit to DATA mode
#ifdef DEBUG
    Serial.println(F("Switching to DATA mode!"));
#endif
    bleModule->setMode(BLUEFRUIT_MODE_DATA);

#ifdef DEBUG
    Serial.println(F("******************************"));
#endif
}

Bluetooth::Button* Bluetooth::AdafruitModule::getControllerButtons()
{
    packetParser::Packet p;
    p.packetBuffer = &packetBuffer[0];
    uint8_t len = packetParser::readPacket(bleModule, BLE_READPACKET_TIMEOUT, p);
    if (len == 0)
        return 0;

    /*Got a packet*/
#ifdef DEBUG
    packetParser::printHex(p.packetBuffer, len);
#endif

    if (p.packetBuffer[1] == 'B')
    {
        uint8_t buttonID = p.packetBuffer[2] - '0';
        boolean buttonState = p.packetBuffer[3] - '0';
        button->buttonID = buttonID;
        button->buttonState = buttonState;
#ifdef DEBUG
        Serial.print("Button ");
        Serial.print(buttonID);
        if (buttonState)
        {
            Serial.println(" pressed");
        }
        else
        {
            Serial.println(" released");
        }
#endif
        return button;
    }

    return 0;
}
