#include <util/crc16.h>
#include <avr/pgmspace.h>

#include "cpul.h"

tCpul_ResultBlock_str Cpul_CreateResultBlock_str(const char * name_c, const tU08 cycles_U08, const tU08 prescaler_U08)
{
  tCpul_ResultBlock_str resultBlock_str;
  strcpy_P(resultBlock_str.name_aC, (PGM_P)name_c);
  resultBlock_str.cycles_U08 = cycles_U08;
  resultBlock_str.prescaler_U08 = prescaler_U08;

  /* Temporary pointer */
  tU08 * data_pU08 = (tU08*)&resultBlock_str;

  for (tU08 byte_U08 = sizeof(tCpul_ResultBlock_str); byte_U08 > sizeof(((tCpul_ResultBlock_str*)0)->crc_U08); byte_U08--)
  {
    resultBlock_str.crc_U08 = _crc8_ccitt_update(resultBlock_str.crc_U08, *data_pU08++);
  }

  return resultBlock_str;
}
