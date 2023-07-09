#include "nrf24l01p_tools.h"

uint8_t nrf24_get_reg_bit(uint8_t reg_data, uint8_t bit_num)
{
    return ((reg_data >> bit_num) & 0x01);
}

void nrf24_set_reg_bit(uint8_t *reg_data, uint8_t bit_num, uint8_t state)
{
    uint8_t _reg[8];
    uint8_t temp_reg_data;

    temp_reg_data = *reg_data;

    for(uint8_t i = 0; i < 8; i++)
        _reg[i] = ((temp_reg_data >> i) & 0x01);
    
    if(bit_num > 7)
        bit_num = 7;

    _reg[bit_num] = state;

    for(uint8_t i = 0; i < 8; i++)
        temp_reg_data = ((temp_reg_data << i) | _reg[i]);
    
    *reg_data = temp_reg_data;
}


/*
void setValuetoByte(uint8_t *reg_addr, uint8_t value, int n)
{
	if(value == 0)
	(*reg_addr) &= ~(0x1 << (n - 1));
	else
	(*reg_addr) |= (0x1 << (n - 1));
}
*/
