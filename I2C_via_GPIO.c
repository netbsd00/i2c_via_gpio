/*
 * I2C_via_GPIO.c
 *
 *  Created on: 2023. 4. 3.
 *      Author: hwyi
 */

#include "I2C_via_GPIO.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#if 1   // 28335 only
#define DELAY_681   0.5   // 681kHz, 1.46us
#define DELAY_599   0.6   // 599kHz, 1.66us
#define DELAY_483   0.8   // 483kHz, 2.06us
#define DELAY_405   1     // 405kHz, 2.46us
#define DELAY_348   1.2   // 348kHz, 2.86us
#define DELAY_306   1.4   // 306kHz, 3.26us
#define DELAY_272   1.6   // 272kHz. 3.66us
#define DELAY_245   1.8   // 245kHz, 4.06us
#define DELAY_223   2     // 223kHz, 4.46us
#endif
#define NULL    0
#define DELAY_FULL    DELAY_US(DELAY_306)
#define DELAY_HALF    DELAY_US(DELAY_599)

#define DIR_IN  0
#define DIR_OUT 1

#define SDA_MUX     GpioCtrlRegs.GPBMUX2.bit.GPIO61     = 0
#define SDA_PU      GpioDataRegs.GPBPUD.bit.GPIO61      = 1
#define SDA_IN      GpioCtrlRegs.GPBDIR.bit.GPIO61      = DIR_IN
#define SDA_OUT     GpioCtrlRegs.GPBDIR.bit.GPIO61      = DIR_OUT
#define SDA_H       GpioDataRegs.GPBSET.bit.GPIO61      = 1
#define SDA_L       GpioDataRegs.GPBCLEAR.bit.GPIO61    = 1
#define SDA_DAT     GpioDataRegs.GPBDAT.bit.GPIO61

#define SCL_MUX     GpioCtrlRegs.GPBMUX2.bit.GPIO60     = 0
#define SCL_PU      GpioDataRegs.GPBPUD.bit.GPIO60      = 1
#define SCL_IN      GpioCtrlRegs.GPBDIR.bit.GPIO60      = DIR_IN
#define SCL_OUT     GpioCtrlRegs.GPBDIR.bit.GPIO60      = DIR_OUT
#define SCL_H       GpioDataRegs.GPBSET.bit.GPIO60      = 1
#define SCL_L       GpioDataRegs.GPBCLEAR.bit.GPIO60    = 1

#define SET_SDA_IN     EALLOW;  SDA_IN;  EDIS;
#define SET_SDA_OUT    EALLOW;  SDA_OUT; EDIS;

#define SEND_ACK    generate_signal_low()
#define SEND_NACK   generate_signal_high()

static inline generate_signal_high()
{
    SDA_H;
    DELAY_HALF;
    SCL_H;
    DELAY_FULL;
    SCL_L;
    DELAY_HALF;
}

static inline generate_signal_low()
{
    SDA_L;
    DELAY_HALF;
    SCL_H;
    DELAY_FULL;
    SCL_L;
    DELAY_HALF;
}

static inline uint8_t check_ack()
{
    uint8_t ack = NACK;

    SET_SDA_IN;
    DELAY_HALF;
    SCL_H;
    DELAY_HALF;
    if(SDA_DAT == 0)
    {
        ack = ACK;
    }
    else
    {
        ack = NACK;
    }
    DELAY_HALF;
    SCL_L;
    DELAY_HALF;

    return ack;
}

void i2vg_init(void)
{
    EALLOW;
    SCL_MUX;
    SCL_OUT;

    SDA_MUX;
    SDA_OUT;
    EDIS;

    SDA_H;
    SCL_H;
}

void i2vg_start()
{
    DELAY_HALF;
    SET_SDA_OUT;
    SDA_L;
    DELAY_HALF;
    SCL_L;
    DELAY_HALF;
}

void i2vg_stop()
{
    DELAY_HALF;
    SET_SDA_OUT;
    SDA_L;
    DELAY_HALF;
    SCL_H;
    DELAY_HALF;
    SDA_H;
    DELAY_HALF;
}

uint8_t i2vg_write(uint8_t byte)
{
    uint8_t i = 0;
    uint8_t ack = NACK;

    SET_SDA_OUT;

    for(i=0; i<8; i++)
    {
        if( (byte & (0x80 >> i)) == 0 )
        {
            generate_signal_low();
        }
        else
        {
            generate_signal_high();
        }
    }

    ack = check_ack();

    return ack;
}

uint8_t i2vg_read()
{
    int8_t i = 0;
    uint8_t data = 0x00;

    SET_SDA_IN;

    for(i=7; i>=0; i--)
    {
        DELAY_HALF;
        SCL_H;
        DELAY_HALF;

        if(SDA_DAT == 1)
        {
            data = data | (0x01 << i);
        }

        DELAY_HALF;
        SCL_L;
        DELAY_HALF;
    }

    SET_SDA_OUT;
    SEND_ACK;

    return data;
}
