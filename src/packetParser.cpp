#include "AdafruitBluetoothModule.h"

/**************************************************************************/
/*!
    @brief  Casts the four bytes at the specified address to a float
*/
/**************************************************************************/

float packetParser::parsefloat(uint8_t *buffer) 
{
  float f;
  memcpy(&f, buffer, 4);
  return f;
}

/**************************************************************************/
/*! 
    @brief  Prints a hexadecimal value in plain characters
    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void packetParser::printHex(const uint8_t * data, const uint32_t numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++) 
  {
    Serial.print(F("0x"));
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
    {
      Serial.print(F("0"));
      Serial.print(data[szPos] & 0xf, HEX);
    }
    else
    {
      Serial.print(data[szPos] & 0xff, HEX);
    }
    // Add a trailing space if appropriate
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(F(" "));
    }
  }
  Serial.println();
}

/**************************************************************************/
/*!
    @brief  Waits for incoming data and parses it
*/
/**************************************************************************/
uint8_t packetParser::readPacket(Adafruit_BLE *ble, uint16_t timeout, packetParser::Packet& packet) 
{
  uint16_t origtimeout = timeout;

  memset(packet.packetBuffer, 0, READ_BUFSIZE);

  while (timeout--) {
    if (packet.replyIdx >= 20) break;
    if ((packet.packetBuffer[1] == 'A') && (packet.replyIdx == PACKET_ACC_LEN))
      break;
    if ((packet.packetBuffer[1] == 'G') && (packet.replyIdx == PACKET_GYRO_LEN))
      break;
    if ((packet.packetBuffer[1] == 'M') && (packet.replyIdx == PACKET_MAG_LEN))
      break;
    if ((packet.packetBuffer[1] == 'Q') && (packet.replyIdx == PACKET_QUAT_LEN))
      break;
    if ((packet.packetBuffer[1] == 'B') && (packet.replyIdx == PACKET_BUTTON_LEN))
      break;
    if ((packet.packetBuffer[1] == 'C') && (packet.replyIdx == PACKET_COLOR_LEN))
      break;
    if ((packet.packetBuffer[1] == 'L') && (packet.replyIdx == PACKET_LOCATION_LEN))
      break;

    while (ble->available()) {
      char c =  ble->read();
      if (c == '!') {
        packet.replyIdx = 0;
      }
      packet.packetBuffer[packet.replyIdx] = c;
      packet.replyIdx++;
      timeout = origtimeout;
    }
    
    if (timeout == 0) break;
    delay(1);
  }

  packet.packetBuffer[packet.replyIdx] = 0;  // null term

  if (!packet.replyIdx)  // no data or timeout 
    return 0;
  if (packet.packetBuffer[0] != '!')  // doesn't start with '!' packet beginning
    return 0;
  
  // check checksum!
  uint8_t xsum = 0;
  uint8_t checksum = packet.packetBuffer[packet.replyIdx-1];
  
  for (uint8_t i=0; i<packet.replyIdx-1; i++) {
    xsum += packet.packetBuffer[i];
  }
  xsum = ~xsum;

  // Throw an error message if the checksum's don't match
#ifdef DEBUG
  if (xsum != checksum)
  {
    Serial.print("Checksum mismatch in packet : ");
    printHex(packet.packetBuffer, packet.replyIdx+1);
    return 0;
  }
#endif
  
  // checksum passed!
  return packet.replyIdx;
}

