#ifndef CPUL_H_
#define CPUL_H_

#include "type.h"

typedef struct
{
    tU08 name_aU08[4]; /**< Module name */
    tU08 prescaler_U08;  /**< Prescaler */
    tU08 cycles_U08;  /**< Number of cycles */
    tU08 crc_U08;  /**< CRC */
} tCpul_ResultBlock_str;

/**
 * @brief      Set start point for cpu load measurement
 *
 * @param  prescalerRegister_U08  Prescaler register
 */
void Cpul_startPoint(tU08 prescalerRegister);
tU08 Cpul_stopPoint_U08(void);
tCpul_ResultBlock_str Cpul_CreateResultBlock_str(char name_c[], tU08 cycles_U08, tU08 prescaler_U08);

#endif /* CPUL_H_ */ 
