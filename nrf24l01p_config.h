#ifndef __NRF24L01P_CONFIG_H__
#define __NRF24L01P_CONFIG_H__

#include "nrf24l01p.h"
/********** NRF24L01+ config **********/

//NRF Command                                                                     Data bytes
#define NRF_NOP                             0xFF    // No Operation
                                                    //
#define NRF_R_REGISTER                      0x00    // Read Command   
                                                    //                                    LSB 1 ~ 5
#define NRF_W_REGISTER                      0x20    // Write Command  
                                                    //                                    LSB 1 ~ 5
#define NRF_R_RX_PAYLOAD                    0x61    // Read   RX-payload
                                                    //                                    LSB 1 ~ 32
#define NRF_W_TX_PAYLOAD                    0xA0    // Write  TX-payload
                                                    //                                    LSB 1 ~ 32
#define NRF_FLUSH_TX                        0xE1    // Flush TX FIFO
                                                    //
#define NRF_FLUSH_RX                        0xE2    // Flush RX FIFO
                                                    //
#define NRF_REUSE_TX_PL                     0xE3    // Used for a PTX device Reuse last transmitted 
                                                    // payload. TX payload reuse is active until 
                                                    // W_TX_PAYLOAD or FLUSH TX is executed. TX 
                                                    // payload reuse must not be activated or 
                                                    // deactivated during package transmission.
                                                    //
#define NRF_R_RX_PL_WID                     0x60    // Read RX payload width.
                                                    // Note: Flush RX FIFO if the read value is 
                                                    // larger than 32 bytes.
                                                    //
#define NRF_W_ACK_PAYLOAD                   0xA8    // "1010 1PPP"  assume "PPP" -> "000"
                                                    // Write Payload to be transmitted together 
                                                    // with ACK packet on PIPE PPP. (PPP valid in 
                                                    // the range from 000 to 101). Maximum three 
                                                    // ACK packet payloads can be pending. Payloads 
                                                    // with same PPP are handled using first 
                                                    // in - first out principle. Write 
                                                    // payload: 1 – 32 bytes. A write operation 
                                                    // always starts at byte 0.
                                                    //                                    LSB 1 ~ 32
#define NRF_W_TX_PAYLOAD_NOACK              0x80    // Disables AUTOACK on this specific packet.
                                                    //                                    LSB 1 ~ 32
//NRF Register

/*** Form ***/
/*
* 
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7]:
* Bit [6]:
* Bit [5]:
* Bit [4]:
* Bit [3]:
* Bit [2]:
* Bit [1]:
* Bit [0]:
*/
/*** Form ***/

#define NRF_CONFIG                          0x00
/*
* Config  
* Configuration Register
*
* Bits        Name          Status                                Reset value   details
* Bit 7:      Reserved                                            [0]           Only '0' allowed
* Bit 6:      MASK_RX_DR    [1]: IRQ inactive   [0]: IRQ active   [0]           RX side receive data packet IRQ switch
* Bit 5:      MASK_TX_DS    [1]: IRQ inactive   [0]: IRQ active   [0]           TX side receive Ack packet IRQ switch
* Bit 4:      MASK_MAX_RT   [1]: IRQ inactive   [0]: IRQ active   [0]           Auto Retransimit failed reaches Maximum time IRQ switch
* Bit 3:      EN_CRC        [1]: CRC enable     [0]: CRC disable  [1]           Forced high if one of the bits in the EN_AA is high
* Bit 2:      CRCO          [1]: 2 bytes        [0]: 1 byte       [0]           CRC encoding scheme
* Bit 1:      PWR_UP        [1]: Power up       [0]: power down   [0]           Power control (soft)
* Bit 0:      PRIM_RX       [1]: RX mode        [0]: TX mode      [0]           Mode select
*/

#define NRF_EN_AA                           0x01
/*
* Enhanced ShockBurst™
* Enable ‘Auto Acknowledgment’ Function Disable this functionality to be compatible with nRF2401, see page 75
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5]:    ENAA_P5       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 5
* Bit [4]:    ENAA_P4       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 4
* Bit [3]:    ENAA_P3       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 3
* Bit [2]:    ENAA_P2       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 2
* Bit [1]:    ENAA_P1       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 1
* Bit [0]:    ENAA_P0       [1]: Enable         [0]: Disable      [1]           Enable auto acknowledgement data pipe 0
*/

#define NRF_EN_RXADDR                       0x02
/*
* EN_RXADDR
* Enabled RX Addresses
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5]:    ERX_P5        [1]: Enable         [0]: Disable      [0]           Enable data pipe 5
* Bit [4]:    ERX_P4        [1]: Enable         [0]: Disable      [0]           Enable data pipe 4
* Bit [3]:    ERX_P3        [1]: Enable         [0]: Disable      [0]           Enable data pipe 3
* Bit [2]:    ERX_P2        [1]: Enable         [0]: Disable      [0]           Enable data pipe 2
* Bit [1]:    ERX_P1        [1]: Enable         [0]: Disable      [1]           Enable data pipe 1
* Bit [0]:    ERX_P0        [1]: Enable         [0]: Disable      [1]           Enable data pipe 0
*/

#define NRF_SETUP_AW                        0x03
/*
* SETUP_AW
* Setup of Address Widths (common for all data pipes)
*
* Bits        Name          Status                                Reset value   details
* Bit [7:2]:  Reserved                                            [0]           Only 0 allowed
* Bit [1:0]:  AW                                                  [11]          RX/TX Address field width
*                           [00]                                                Illegal
*                           [01]                                                3 bytes
*                           [10]                                                4 bytes
*                           [11]                                                5 bytes
*/

#define NRF_SET_RETR                        0x04
/*
* SETUP_RETR
* Setup of Automatic Retransmission
*
* Bits        Name          Status                                Reset value   details
* Bit [7:4]:  ARD                                                 [0000]        Auto Retransmit Delay 
*                           [0000]                                              Wait 250us
*                           [0001]                                              wait 500us
*                           [0010]                                              wait 750us
*                           [....]                                              ....
*                           [1111]                                              wait 4000us
* Bit [3:0]: ARC                                                  [0011]        Auto Retransmit Count
*                           [0000]                                              Re-transmit disable
*                           [0001]                                              Up to 1 Re-Transmit on fail of AA
*                           [0010]                                              Up to 2 Re-Transmit on fail of AA
*                           [....]                                              ....
*                           [1111]                                              Up to 15 Re-Transmit on fail of AA
*/

#define NRF_RF_CH                           0x05
/*
* RF_CH
* RF Channel
*
* Bits        Name          Status                                Reset value   details
* Bit [7]:    Reserved                                            [0]           Only '0' allowed
* Bit [6:0]:  RF_CH                                               [0000010]     Sets the frequency channel nRF24L01+ operates on
*/

#define NRF_RF_SETUP                        0x06
/*
* RF_SETUP
* RF Setup Register
*
* Bits        Name          Status                                Reset value   details
* Bit [7]:    CONT_WAVE     [1]: Enable         [0]: Disable      [0]           Enables continuous carrier transmit when high.
* Bit [6]:    Reserved                                            [0]           Only '0' allowed
* Bit [5]:    RF_DR_LOW     [1]: Enanle         [0]: Disable      [0]           Set RF Data Rate to 250kbps. See RF_DR_HIGH for encoding.
* Bit [4]:    PLL_LOCK      [1]: Enable         [0]: Disable      [0]           Force PLL lock signal. Only used in test
* Bit [3]:    RF_DR_HIGH    [0]: 1Mbps          [1]: 2Mbps        [1]           [RF_DR_LOW, RF_DR_HIGH]: ‘00’ – 1Mbps/‘01’ – 2Mbps/‘10’ – 250kbps/‘11’ – Reserved
* Bit [2:1]:  RF_PWR                                              [11]          Set RF output power in TX mode
*                           [00]                                                '00' – -18dBm
*                           [01]                                                '01' – -12dBm
*                           [10]                                                '10' –  -6dBm
*                           [11]                                                '11' –   0dBm
* Bit [0]:    Obsolete                                                          Don't care
*/

#define NRF_STATUS                          0x07
/*
* STATUS
* Status Register (In parallel to the SPI command word applied on the MOSI pin, the STATUS register is shifted serially out on the MISO pin)
*
* Bits        Name          Status                                Reset value   details
* Bit [7]:    Reserved                                                          Only '0' allowed
* Bit [6]:    RX_DR         [1]: Clear bit      [0]: No IRQ       [0]           Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO
* Bit [5]:    TX_DS         [1]: Clear bit      [0]: No IRQ       [0]           Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. 
*                                                                               If AUTO_ACK is activated, this bit is set high only when ACK is received.
* Bit [4]:    MAX_RT        [1]: Clear bit      [0]: No IRQ       [0]           Maximum number of TX retransmits interrupt. If MAX_RT is asserted it must 
*                                                                               be cleared to enable further communication.
* Bit [3:1]:  RX_P_NO                                             [111] R       Data pipe number for the payload available for reading from RX_FIFO.
                            [000 ~ 101]                                         Data pipe Number. (0 ~ 5)
* Bit [0]:    TX_FULL       [1]: TX Full        [0]: Available    [0] R         TX FIFO Full flag.
*/

#define NRF_OBSERVE_TX                      0x08
/*
* OBSERVE_TX  
* Transmit observe register
*
* Bits        Name          Status                                Reset value   details
* Bit [7:4]:  PLOS_CNT                                            [0] R         Count lost packets. The counter is overflow protected to 15, 
                                                                                and discontinues at max until reset. The counter is reset by writing to RF_CH.
* Bit [3:0]:  ARC_CNT                                             [0] R         Count retransmitted packets. The counter is reset when transmission of a new packet starts.
*/

#define NRF_RPD                             0x09
/*
* RPD
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7:1}:  Reserved                                            [000000] R    Only '000000' allowed
* Bit [0]:    RPD                                                 [0] R         Received Power Detector. This register is called CD (Carrier Detect) in the nRF24L01. 
*                                                                               The name is different in nRF24L01+ due to the different input power level threshold 
*                                                                               for this bit. See section 6.4 on page 25.
*/

#define NRF_RX_ADDR_P0                      0x0A
/*
* RX_ADDR_P0
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [39:0]  RX_ADDR_P0                                          0xE7E7E7E7E7  Receive address data pipe 0. 5 Bytes maximum length. 
*                                                                               (LSByte is written first. Write the number of bytes defined by SETUP_AW)
*/

#define NRF_RX_ADDR_P1                      0x0B
/*
* RX_ADDR_P1
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [39:0]  RX_ADDR_P1                                          0xC2C2C2C2C2  Receive address data pipe 1. 5 Bytes maximum length. 
*                                                                               (LSByte is written first. Write the number of bytes defined by SETUP_AW)
*/

#define NRF_RX_ADDR_P2                      0x0C
/*
* RX_ADDR_P2
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7:0]:  RX_ADDR_P2                                          [0xC3]        Receive address data pipe 2. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
*/

#define NRF_RX_ADDR_P3                      0x0D
/*
* RX_ADDR_P3
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7:0]:  RX_ADDR_P3                                          [0xC4]        Receive address data pipe 3. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
*/

#define NRF_RX_ADDR_P4                      0x0E
/*
* RX_ADDR_P4
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7:0]:  RX_ADDR_P4                                          [0xC5]        Receive address data pipe 4. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
*/

#define NRF_RX_ADDR_P5                      0x0F
/*
* RX_ADDR_P5
* 
*
* Bits        Name          Status                                Reset value   details
* Bit [7:0]:  RX_ADDR_P5                                          [0xC6]        Receive address data pipe 5. Only LSB. MSBytes are equal to RX_ADDR_P1[39:8]
*/

#define NRF_TX_ADDR                         0x10
/*
* TX_ADDR
* Transmit address. Used for a PTX device only. (LSByte is written first) 
* Set RX_ADDR_P0 equal to this address to handle automatic acknowledge if this is a PTX device with Enhanced ShockBurst™ enabled. See page 75.
*
* Bits        Name          Status                                Reset value   details
* Bit [39:0]  TX_ADDR                                             0xE7E7E7E7E7  
*/

#define NRF_RX_PW_P0                        0x11
/*
* RX_PW_P0
* Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P0                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_RX_PW_P1                        0x12
/*
* RX_PW_P0
* Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P0                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_RX_PW_P2                        0x13
/*
* RX_PW_P2
* Number of bytes in RX payload in data pipe 2 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P2                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_RX_PW_P3                        0x14
/*
* RX_PW_P3
* Number of bytes in RX payload in data pipe 3 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P3                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_RX_PW_P4                        0x15
/*
* RX_PW_P4
* Number of bytes in RX payload in data pipe 4 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P4                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_RX_PW_P5                        0x16
/*
* RX_PW_P5
* Number of bytes in RX payload in data pipe 5 (1 to 32 bytes).
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5:0]:  RX_PW_P5                                            [000000]
*                           [000001]                                            1 byte
*                           [000010]                                            2 bytes
*                           [000011]                                            3 bytes
*                           [100000]                                            32 bytes
*/

#define NRF_FIFO_STATUS                     0x17
/*
* FIFO_STATUS
* FIFO Status Register
*
* Bits        Name          Status                                Reset value   details
* Bit [7]:    Resvered                                            [0]           Only '0' allowed
* Bit [6]:    T_REUSE       [1]: Reuse         [0]: Reset         [0] R         Used for a PTX device Pulse the rfce high for at least 10µs to Reuse last transmitted payload. 
*                                                                               TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed. 
*                                                                               TX_REUSE is set by the SPI command REUSE_TX_PL, 
*                                                                               and is reset by the SPI commands W_TX_PAYLOAD or FLUSH TX
* Bit [5]:    TX_FULL       [1]: Full          [0]: Available     [0] R         TX FIFO full flag.
* Bit [4]:    TX_EMPTY      [1]: TX Empty      [0]: Data in FIFO  [1] R         TX FIFO empty flag.
* Bit [3:2]:  Reserved                                            [00]          Only '00' allowed
* Bit [1]:    RX_FULL       [1]: Full          [0]: Available     [0] R         RX FIFO full flag.
* Bit [0]:    RX_EMPTY      [1]: RX Empty      [0]: Data in FIFO  [1] R         RX FIFO empty flag.
*/

#define NRF_DYNPD                           0x1C
/*
* DYNPD
* Enable dynamic payload length
*
* Bits        Name          Status                                Reset value   details
* Bit [7:6]:  Reserved                                            [00]          Only '00' allowed
* Bit [5]:    DPL_P5        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 5. (Requires EN_DPL and ENAA_P5)
* Bit [4]:    DPL_P4        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 4. (Requires EN_DPL and ENAA_P4)
* Bit [3]:    DPL_P3        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 3. (Requires EN_DPL and ENAA_P3)
* Bit [2]:    DPL_P2        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 2. (Requires EN_DPL and ENAA_P2)
* Bit [1]:    DPL_P1        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 1. (Requires EN_DPL and ENAA_P1)
* Bit [0]:    DPL_P0        [1]: Enable        [0]: Disable       [0]           Enable dynamic payload length data pipe 0. (Requires EN_DPL and ENAA_P0)
*/

#define NRF_FEATURE                         0x1D
/*
* FEATURE
* Feature Register
*
* Bits        Name          Status                                Reset value   details
* Bit [7:3]:  Reserved                                            [00000]       Only '00000' allowed
* Bit [2]:    EN_DPL        [1]: Enable        [0]: Disable       [0]           Enables Dynamic Payload Length
* Bit [1]:    EN_ACK_PAY    [1]: Enable        [0]: Disable       [0]           Enables Payload with ACK
* Bit [0]:    EN_DYN_ACK    [1]: Enable        [0]: Disable       [0]           Enables the W_TX_PAYLOAD_NOACK command
*/

/**************************************/

/***** Customize Preference *****/

//NRF Enbale
#define NRF_Enable                          ((uint8_t) 0x01)
#define NRF_Disable                         ((uint8_t) 0x00)

//Auto Retransmit Delay
#define NRF_RETRANS_DELAY_250us             ((uint8_t) 0x00)
#define NRF_RETRANS_DELAY_500us             ((uint8_t) 0x10)
#define NRF_RETRANS_DELAY_750us             ((uint8_t) 0x20)
#define NRF_RETRANS_DELAY_1000us            ((uint8_t) 0x30)
#define NRF_RETRANS_DELAY_2000us            ((uint8_t) 0x70)
#define NRF_RETRANS_DELAY_4000us            ((uint8_t) 0xF0)

//Auto Retransmit Count
#define NRF_RETRANS_COUNT_Disable           ((uint8_t) 0x00)
#define NRF_RETRANS_COUNT_Up_to_1           ((uint8_t) 0x01)
#define NRF_RETRANS_COUNT_Up_to_2           ((uint8_t) 0x02)
#define NRF_RETRANS_COUNT_Up_to_3           ((uint8_t) 0x03)
#define NRF_RETRANS_COUNT_Up_to_5           ((uint8_t) 0x05)
#define NRF_RETRANS_COUNT_Up_to_10          ((uint8_t) 0x0A)
#define NRF_RETRANS_COUNT_Up_to_15          ((uint8_t) 0x0F)

//RF Data Rate
#define NRF_RF_RATE_1Mbps                   ((uint8_t) 0x00)
#define NRF_RF_RATE_2Mbps                   ((uint8_t) 0x08)
#define NRF_RF_RATE_250kbps                 ((uint8_t) 0x10)

//RF Output Power
#define NRF_RF_POWER_N18dBm                 ((uint8_t) 0x00)    // (0 0 0 0) low 4
#define NRF_RF_POWER_N12dBm                 ((uint8_t) 0x02)    // (0 0 1 0) low 4
#define NRF_RF_POWER_N6dBm                  ((uint8_t) 0x04)    // (0 1 0 0) low 4
#define NRF_RF_POWER_0dBm                   ((uint8_t) 0x06)    // (0 1 1 0) low 4

//Interrupts
#define NRF_IRQ_RX_FIFO_Ready               ((uint8_t) 0x40)
#define NRF_IRQ_TX_FIFO_Sent                ((uint8_t) 0x20)
#define NRF_IRQ_MAX_Retransmit              ((uint8_t) 0x01)

//Receive Power
#define NRF_Channel_Signal_qualified        ((uint8_t) 0x01)
#define NRF_Channel_Signal_unqualified      ((uint8_t) 0x00)

//Dynamic payload state
#define NRF_Dynamic_payload_Enable          NRF_Enable
#define NRF_Dynamic_payload_Disable         NRF_Disable


/********************************/


#endif
