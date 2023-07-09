#ifndef __NRF24L01P_TOOLS_H__
#define __NRF24L01P_TOOLS_H__

//#include "nrf24l01p.h"

#include <stdint.h>

uint8_t nrf24_get_reg_bit(uint8_t reg_data, uint8_t bit_num);
void nrf24_set_reg_bit(uint8_t *reg_data, uint8_t bit_num, uint8_t state);

#endif
