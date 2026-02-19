#pragma once
#include <OneWireHub.h>
#include <DS2502.h>
#include "chg_emu.h"

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

class chg_emu_dell_c final : public chg_emu_c
{
public:
  chg_emu_dell_c(uint8_t pin);
  ~chg_emu_dell_c();

  void setup() override;
  void loop() override;

  void set_param(const uint32_t millivolt, const uint32_t milliamp) override;
  void enable() override;
  void disable() override;

protected:
  static void int_to_char(uint32_t milli_num, char str[3]);
  static void calculate_dell_crc(const char *string_40byte, uint8_t *crc_l, uint8_t *crc_h);
  static void make_dell_chg_id(dell_chg_id_t &id, const uint32_t millivolt, const uint32_t milliamp);

private:
  OneWireHub *_hub;
  DS2502 *_dell_CH;
};