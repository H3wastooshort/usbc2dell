#include "chg_emu_dell.hpp"
#include <Arduino.h>
#include <OneWireHub.h>
#include <DS2502.h>

constexpr uint8_t DELL_CHG_LEN = sizeof(dell_chg_id_t);
static_assert(DELL_CHG_LEN == 42);

chg_emu_dell_c::chg_emu_dell_c(uint8_t pin) {
    this->_hub = new OneWireHub(pin);
    this->_dell_CH = new DS2502(0x28, 0x0D, 0x01, 0x08, 0x0B, 0x02, 0x0A); // address does not matter, laptop uses skipRom -> note that therefore only one peripheral device is allowed on the bus
}

chg_emu_dell_c::~chg_emu_dell_c() {
    this->disable();
    delete this->_dell_CH;
    delete this->_hub;
}

void chg_emu_dell_c::setup() {
    // do nothing
}

void chg_emu_dell_c::loop() {
    this->_hub->poll();
}

void chg_emu_dell_c::set_param(const uint32_t millivolt, const uint32_t milliamp) {
    dell_chg_id_t id;
    make_dell_chg_id(id, millivolt, milliamp);
    this->_dell_CH->writeMemory(reinterpret_cast<uint8_t *>(&id), 42);
}
void chg_emu_dell_c::enable() {
    this->_hub->attach(*this->_dell_CH);
}
void chg_emu_dell_c::disable() {
    this->_hub->detach(*this->_dell_CH);
}

void chg_emu_dell_c::int_to_char(uint32_t milli_num, char str[3]) {
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
void chg_emu_dell_c::calculate_dell_crc(const char *string_40byte, uint8_t *crc_l, uint8_t *crc_h) {
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

void chg_emu_dell_c::make_dell_chg_id(dell_chg_id_t &id, const uint32_t millivolt, const uint32_t milliamp)
{
    uint32_t decivolt = millivolt / 100;
    uint32_t deciamp = milliamp / 100;
    uint32_t watt = (decivolt * deciamp) / 100;
    chg_emu_dell_c::int_to_char(watt, id.wattage);
    chg_emu_dell_c::int_to_char(decivolt, id.voltage);
    chg_emu_dell_c::int_to_char(deciamp, id.amperage);

    chg_emu_dell_c::calculate_dell_crc(reinterpret_cast<const char *>(&id), &id.crc[0], &id.crc[1]);
}