#include "nrf24l01p.h"


// esp32
char nrf_send[] = {'3', '2', 'f', 'w', 'b'};
// stm32
char nrf_receive[] = {'s', 't', 'm', '3', '2'};

uint8_t nrf_temp_tx_ram[32];
uint8_t nrf_temp_rx_ram[32];


void NRF24_Init(uint8_t mode)
{
    for(uint8_t i = 0; i < 32; i++)
    {
        nrf_temp_tx_ram[i] = 0x00;
        nrf_temp_rx_ram[i] = 0x00;
    }

    if(mode == NRF_PTX)
    {
        // For esp32c3FN4
        NRF24_PowerDown();

        spi_write_byte(NRF_W_REGISTER | NRF_SETUP_AW, 0x03);            // Enable 5 bytes length address
        
        NRF24_Set_RX_Payload_byte_num(0, 32);

        NRF24_Set_TX_Address(nrf_send);

        NRF24_Set_RX_Address_pipe_0(nrf_send);

        spi_write_byte(0x1C | NRF_W_REGISTER, 0x01);
        spi_write_byte(0x1D | NRF_W_REGISTER, 0x06);

        NRF24_TX_Init(NRF_Channel, NRF_RADIO_POWER, NRF_RADIO_RATE, NRF_RETRANS_DELAY, NRF_RETRANS_COUNT);

        NRF24_Clear_All_IRQ();
        NRF24_Flush_RX_FIFO();
        NRF24_Flush_TX_FIFO();
    }
    else
    {
        // For stm32f411CEU6
        NRF24_PowerDown();
        
        spi_write_byte(NRF_W_REGISTER | NRF_SETUP_AW, 0x03);            // Enable 5 bytes length address

        NRF24_Set_RX_Payload_byte_num(0, 32);

        NRF24_Set_RX_Address_pipe_0(nrf_send);

        spi_write_byte(0x1C | NRF_W_REGISTER, 0x01);
        spi_write_byte(0x1D | NRF_W_REGISTER, 0x06);

        NRF24_RX_Init(NRF_Channel, NRF_RADIO_POWER, NRF_RADIO_RATE, NRF_RETRANS_DELAY, NRF_RETRANS_COUNT);

        NRF24_Clear_All_IRQ();
        NRF24_Flush_RX_FIFO();
        NRF24_Flush_TX_FIFO();
    }
}

void NRF24_TX_Init(uint8_t channel, uint8_t radio_power, uint8_t radio_speed, uint8_t retrans_delay, uint8_t retrans_count)
{
    spi_write_byte(NRF_W_REGISTER | NRF_EN_AA, 0x01);               // Enable auto ack on pipe 0, 1

    spi_write_byte(NRF_W_REGISTER | NRF_EN_RXADDR, 0x01);           // Enable rx pipe 0, 1

    spi_write_byte(NRF_W_REGISTER | NRF_SET_RETR,                   // Set Auto Retransmit Delay
                    retrans_delay | retrans_count);                 // Set Auto Retransmit Count

    NRF24_Set_RF_Channel(channel);                                  // Set RF Channel

    NRF24_Set_RF_Rate(radio_speed);                                 // Set RF Radio speed

    NRF24_Set_RF_power(radio_power);                                // Set RF Radio power

    spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x0e);              // Set to tx mode, IRQ TX Active IRQ MAX Active, 1 byte CRC, Power down
}

void NRF24_RX_Init(uint8_t channel, uint8_t radio_power, uint8_t radio_speed, uint8_t retrans_delay, uint8_t retrans_count)
{
    spi_write_byte(NRF_W_REGISTER | NRF_EN_AA, 0x01);               // Enable auto ack on pipe 0, 1

    spi_write_byte(NRF_W_REGISTER | NRF_EN_RXADDR, 0x01);           // Enable rx pipe 0, 1

    spi_write_byte(NRF_W_REGISTER | NRF_SET_RETR,                   // Set Auto Retransmit Delay
                retrans_delay | retrans_count);                     // Set Auto Retransmit Count

    NRF24_Set_RF_Channel(channel);                                  // Set RF Channel

    NRF24_Set_RF_Rate(radio_speed);                                 // Set RF Radio speed
    
    NRF24_Set_RF_power(radio_power);                                // Set RF Radio power

    spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x0f);              // Set to rx mode, IRQ RX Active IRQ MAX Active, 1 byte CRC, Power down
}


void NRF24_PTX_IRQHandler(void)
{
    NRF24_Unload_payload(nrf_temp_rx_ram);

    NRF24_Flush_TX_FIFO();
    NRF24_Flush_RX_FIFO();

    NRF24_Clear_All_IRQ();
}

void NRF24_PRX_IRQHandler(void)
{
    //printf("In callback\n");
    uint8_t irq, pld = 0;
    NRF24_Get_Status(&irq);
    //printf("status: %d\n", irq);

    pld = NRF24_Unload_payload(nrf_temp_rx_ram);

    if(pld)
        memcpy(&nrf_send_raw, nrf_default_signal, 32);

    data_exchange_lock_send_ack = 1;
    memcpy(nrf_temp_tx_ram, &nrf_receive_raw, 32);
    data_exchange_lock_send_ack = 0;
    NRF24_Write_RX_ACK_payload(0, nrf_temp_tx_ram);

    if(irq == 96 && pld == 0)
    {
        data_exchange_lock_receive = 1;

        memcpy(&nrf_send_raw, nrf_temp_rx_ram, 32);

        data_exchange_lock_receive = 0;
    }
    
    //NRF24_Flush_TX_FIFO();
    NRF24_Flush_RX_FIFO();

    NRF24_Clear_All_IRQ();
}


void NRF24_Load_payload(uint8_t *data)
{
    NRF24_Flush_TX_FIFO();
    NRF24_Write_TX_payload(data);
}

uint8_t NRF24_Unload_payload(uint8_t *data)
{
    if(NRF24_Read_RX_payload_length() != 0)
    {
        NRF24_Read_RX_payload(data);
    }
    else
    {
        NRF24_Flush_RX_FIFO();

        for(uint8_t i = 0; i < 32; i++)
        {
            *data = 0xff;
            
            data++;
        }

        return NRF_Error;
    }

    return NRF_Success;
}


/*
* Power Up device
*/
void NRF24_PowerUp(void)
{
    uint8_t mode;

    spi_read_byte(NRF_R_REGISTER | NRF_CONFIG, &mode, NRF_NOP);
    
    if(nrf24_get_reg_bit(mode, 0) == NRF_PTX)
        spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x0A);
    else
        spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x0B);
}

/*
* Power Down devide
*/
void NRF24_PowerDown(void)
{
    uint8_t mode;

    spi_read_byte(NRF_R_REGISTER | NRF_CONFIG, &mode, NRF_NOP);
    
    if(nrf24_get_reg_bit(mode, 0) == NRF_PTX)
        spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x08);
    else
        spi_write_byte(NRF_W_REGISTER | NRF_CONFIG, 0x09);
}

/*
* Set the frequency channel
*   0 ~ 127
*/
void NRF24_Set_RF_Channel(uint8_t freq)
{
    freq += 2;

    spi_write_byte(NRF_W_REGISTER | NRF_RF_CH, freq);
}

/*
* Get the frequency channel
*   0 ~ 127
*/
void NRF24_Get_RF_Channel(uint8_t *rf_freq)
{
    uint8_t rf_temp_freq;

    spi_read_byte(NRF_R_REGISTER | NRF_RF_CH, &rf_temp_freq, NRF_NOP);

    *rf_freq = rf_temp_freq - 2;
}

/*
* Set RF Data Rate
*/
void NRF24_Set_RF_Rate(uint8_t rf_rate)
{
    uint8_t rf_power;

    NRF24_Get_RF_power(&rf_power);

    spi_write_byte(NRF_W_REGISTER | NRF_RF_SETUP, rf_rate | rf_power);
}

/*
* Get RF Data Rate
*/
void NRF24_Get_RF_Rate(uint8_t *rf_rate)
{
    uint8_t rf_power, temp_rf_power = 0x00;
	
    spi_read_byte(NRF_R_REGISTER | NRF_RF_SETUP, &rf_power, NRF_NOP);

    temp_rf_power = temp_rf_power | (nrf24_get_reg_bit(rf_power, 5) << 5) | 
                                    (nrf24_get_reg_bit(rf_power, 3) << 3);
    
    *rf_rate = temp_rf_power;
}

/*
* Set RF output power
*/
void NRF24_Set_RF_power(uint8_t rf_power)
{
    uint8_t rf_rate;

    NRF24_Get_RF_Rate(&rf_rate);

    spi_write_byte(NRF_W_REGISTER | NRF_RF_SETUP, rf_rate | rf_power);
}

/*
* Get RF output power
*/
void NRF24_Get_RF_power(uint8_t *rf_power)
{
    uint8_t rf_rate, temp_rf_rate = 0x00;
    spi_read_byte(NRF_R_REGISTER | NRF_RF_SETUP, &rf_rate, NRF_NOP);

    temp_rf_rate = temp_rf_rate | (nrf24_get_reg_bit(rf_rate, 2) << 2) | 
                                        (nrf24_get_reg_bit(rf_rate, 1) << 1);

    *rf_power = temp_rf_rate;
}

/*
* Get NRF IRQ bits
*/
uint8_t NRF24_Get_IRQ_bits(void)
{
    uint8_t rf_status;
    spi_read_byte(NRF_R_REGISTER | NRF_STATUS, &rf_status, NRF_NOP);
    
    if(nrf24_get_reg_bit(rf_status, 6) == 1)
        return NRF_IRQ_RX_FIFO_Ready;
    else if(nrf24_get_reg_bit(rf_status, 5) == 1)
        return NRF_IRQ_TX_FIFO_Sent;
    else if(nrf24_get_reg_bit(rf_status, 4) == 1)
        return NRF_IRQ_MAX_Retransmit;
    else
        return rf_status;
}

/*
* Get NRF Status
*/
void NRF24_Get_Status(uint8_t *rf_status)
{
    spi_read_byte(NRF_R_REGISTER | NRF_STATUS, rf_status, NRF_NOP);
}


/*
* Clear RX IRQ bit
*/
void NRF24_Clear_IRQ_RX_Ready(void)
{
    uint8_t rf_status;
    NRF24_Get_Status(&rf_status);

    spi_write_byte(NRF_W_REGISTER | NRF_STATUS, rf_status | 0x40);
}

/*
* Clear TX IRQ bit
*/
void NRF24_Clear_IRQ_TX_Sent(void)
{
    uint8_t rf_status;
    NRF24_Get_Status(&rf_status);

    spi_write_byte(NRF_W_REGISTER | NRF_STATUS, rf_status | 0x20);
}

/*
* Clear Max Trans IRQ bit
*/
void NRF24_Clear_IRQ_Max_transmit(void)
{
    uint8_t rf_status;
    NRF24_Get_Status(&rf_status);

    spi_write_byte(NRF_W_REGISTER | NRF_STATUS, rf_status | 0x10);
}

/*
* Clear All IRQ bits
*/
void NRF24_Clear_All_IRQ(void)
{
    spi_write_byte(NRF_W_REGISTER | NRF_STATUS, 0x70);
}

/*
* Get NRF lost packets
*/
void NRF24_Get_Lost_packets(uint8_t *lost_pack)
{
    uint8_t rf_observe, temp_rf_observe = 0x00;
    spi_read_byte(NRF_R_REGISTER | NRF_OBSERVE_TX, &rf_observe, NRF_NOP);

    temp_rf_observe = temp_rf_observe | (nrf24_get_reg_bit(rf_observe, 7) << 3) |
                                        (nrf24_get_reg_bit(rf_observe, 6) << 2) |
                                        (nrf24_get_reg_bit(rf_observe, 5) << 1) |
                                        (nrf24_get_reg_bit(rf_observe, 4) << 0);
    *lost_pack = temp_rf_observe;
}

/*
* Get NRF24 resend times
*/
void NRF24_Get_Resend_times(uint8_t *resend_time)
{
    uint8_t rf_observe, temp_rf_observe = 0x00;
    spi_read_byte(NRF_R_REGISTER | NRF_OBSERVE_TX, &rf_observe, NRF_NOP);

    temp_rf_observe = temp_rf_observe | (nrf24_get_reg_bit(rf_observe, 3) << 3) |
                                        (nrf24_get_reg_bit(rf_observe, 2) << 2) |
                                        (nrf24_get_reg_bit(rf_observe, 1) << 1) |
                                        (nrf24_get_reg_bit(rf_observe, 0) << 0);
    *resend_time = temp_rf_observe;
}

/*
* Get Signal quality
*/
uint8_t NRF24_Get_Receive_power_detector(void)
{
    uint8_t rf_rpd;
    spi_read_byte(NRF_R_REGISTER | NRF_RPD, &rf_rpd, NRF_NOP);

    return rf_rpd;
}

/*
* Set NRF RX address for pipe 0
*/
void NRF24_Set_RX_Address_pipe_0(char *addr)
{
    spi_write_bytes(NRF_W_REGISTER | NRF_RX_ADDR_P0, (uint8_t *) addr, 5);
}

/*
* Set NRF RX address for pipe 1
*/
void NRF24_Set_RX_Address_pipe_1(char *addr)
{
    spi_write_bytes(NRF_W_REGISTER | NRF_RX_ADDR_P1, (uint8_t *) addr, 5);
}

/*
* Set NRF RX address for pipe 2 to 5
*/
void NRF24_Set_RX_Address_pipe_2_to_5(char addr_p2, char addr_p3, char addr_p4, char addr_p5)
{
    spi_write_byte(NRF_W_REGISTER | NRF_RX_ADDR_P2, addr_p2);
    spi_write_byte(NRF_W_REGISTER | NRF_RX_ADDR_P3, addr_p3);
    spi_write_byte(NRF_W_REGISTER | NRF_RX_ADDR_P4, addr_p4);
    spi_write_byte(NRF_W_REGISTER | NRF_RX_ADDR_P5, addr_p5);
}

/*
* Set NRF TX Address
*/
void NRF24_Set_TX_Address(char *addr)
{
    spi_write_bytes(NRF_W_REGISTER | NRF_TX_ADDR, (uint8_t *) addr, 5);
}

/*
* Set number of bytes inRX payload
*/
void NRF24_Set_RX_Payload_byte_num(uint8_t pipe_num, uint8_t byte_num)
{
    if(pipe_num > 5)
        return;
    
    if(byte_num > 32)
        byte_num = 0x00;

    spi_write_byte(NRF_W_REGISTER | (NRF_RX_PW_P0 + pipe_num), byte_num);
}

/*
* Get FIFO status
*/
uint8_t NRF24_Get_FIFO_Status(void)
{
    uint8_t re_fifo_status;
    spi_read_byte(NRF_R_REGISTER | NRF_FIFO_STATUS, &re_fifo_status, NRF_NOP);

    return re_fifo_status;
}

/*
* Get special NRF features
*/
void NRF24_Get_Feature(uint8_t *rf_feature)
{
    spi_read_byte(NRF_R_REGISTER | NRF_FEATURE, rf_feature, NRF_NOP);
}

/*
* Read RX payload
*/
void NRF24_Read_RX_payload(uint8_t *data)
{
    spi_read_bytes(NRF_R_REGISTER | NRF_R_RX_PAYLOAD, data, 32, NRF_NOP);
}

/*
* Write TX payload
*/
void NRF24_Write_TX_payload(uint8_t *data)
{
    spi_write_bytes(NRF_W_TX_PAYLOAD, data, 32);
}

/*
* Flush TX FIFO
*/
void NRF24_Flush_TX_FIFO(void)
{
    spi_write_byte(NRF_W_REGISTER | NRF_FLUSH_TX, NRF_NOP);
}

/*
* Flush RX FIFO
*/
void NRF24_Flush_RX_FIFO(void)
{
    spi_write_byte(NRF_W_REGISTER | NRF_FLUSH_RX, NRF_NOP);
}

/*
* Read RX payload width
*/
uint8_t NRF24_Read_RX_payload_length(void)
{
    uint8_t rf_payload;
    spi_read_byte(NRF_R_REGISTER | NRF_R_RX_PL_WID, &rf_payload, NRF_NOP);

    if(rf_payload > 32)
    {
        NRF24_Flush_RX_FIFO();
        return 0;
    }
    else return rf_payload;
}

/*
* Write Ack payload
*/
void NRF24_Write_RX_ACK_payload(uint8_t pipe_num, uint8_t *data)
{   
    if(pipe_num > 5)
        return;
    
    spi_write_bytes(NRF_W_REGISTER | (NRF_W_ACK_PAYLOAD + pipe_num), data, 32);
}

