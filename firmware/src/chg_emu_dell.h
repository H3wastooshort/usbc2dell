#include <Arduino.h>
#include <OneWireHub.h>
#include <DS2502.h>
#include "chg_emu.h"

class dell_chg_emu_c final : public chg_emu_c
{
public:
  dell_chg_emu_c(uint8_t pin)
  {
    this->_hub = &OneWireHub(pin);
    this->_dell_CH = &DS2502(0x28, 0x0D, 0x01, 0x08, 0x0B, 0x02, 0x0A); // address does not matter, laptop uses skipRom -> note that therefore only one peripheral device is allowed on the bus
  }

  void loop()
  {
    this->_hub->poll();
  }
  void set_param(const uint32_t millivolt, const uint32_t milliamp)
  {
    dell_chg_id_t id;
    make_dell_chg_id(id, millivolt, milliamp);
    this->_dell_CH->writeMemory(reinterpret_cast<uint8_t *>(&id), 42);
  }
  void enable()
  {
    this->_hub->attach(*this->_dell_CH);
  }
  void disable()
  {
    this->_hub->detach(*this->_dell_CH);
  }

protected:
  struct dell_chg_id_t
  {
    const char mfg[4] = {'D', 'E', 'L', 'L'};
    const char type[4] = {'0', '0', 'A', 'C'};
    char wattage[3] = {'0', '0', '0'};
    char voltage[3] = {'0', '0', '0'};
    char amperage[3] = {'0', '0', '0'};
    char sn[23] = {'A', 'R', 'L', 'E', 'H', 'Q', '5', '6', 'S', 'C', 'Q', 'K', '2', 'G', 'M', 'H', 'W', '2', 'B', 'L', 'E', 'I', 'D'};
    uint8_t crc[2] = {0, 0};
  };

  const uint8_t DELL_CHG_LEN = sizeof(dell_chg_id_t); // must be 42

  static void dell_chg_num2str(uint32_t milli_num, char str[3])
  {
    for (uint8_t i = 0; i < 3; i++)
    {
      milli_num /= pow(10, 2 - i); // divide by 100 10 1
      milli_num %= 10;             // leave only one numeral
      str[i] = '0' + milli_num;    // ascii code for 0 plus a number below 10 results in ASCII code for that number
    }
  }

  /* Taken from https://github.com/orgua/OneWireHub/tree/main/examples/DS2502_DELLCHG */
  // This function assumes that the supplied string is exactly 40 bytes.
  // crc_l and crc_h are the pointers to the lower and higher bytes of the calculated CRC16, respectively.
  static void dell_chg_crc(const char *string_40byte, uint8_t *crc_l, uint8_t *crc_h)
  {
    uint32_t crc = 0;
    for (int offset = 0; offset < 40; offset++)
    {
      uint8_t byte = string_40byte[offset];
      crc ^= byte;
      for (int i = 0; i < 8; i++)
      {
        if (crc & 1)
          crc = (crc >> 1) ^ 0xA001;
        else
          crc >>= 1;
      }
    }
    *crc_l = crc & 0xFF;
    *crc_h = (crc >> 8) & 0xFF;
  }

  static void make_dell_chg_id(dell_chg_id_t &id, const uint32_t millivolt, const uint32_t milliamp)
  {
    uint32_t decivolt = millivolt / 100;
    uint32_t deciamp = milliamp / 100;
    uint32_t watt = (decivolt * deciamp) / 100;
    dell_chg_num2str(watt, id.wattage);
    dell_chg_num2str(decivolt, id.voltage);
    dell_chg_num2str(deciamp, id.amperage);

    dell_chg_crc(reinterpret_cast<const char *>(&id), &id.crc[0], &id.crc[1]);
  }

private:
  OneWireHub *_hub;
  DS2502 *_dell_CH;
};