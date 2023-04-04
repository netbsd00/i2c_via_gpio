/*
 * main.c
 *
 *  Created on: 2023. 4. 3.
 *      Author: hwyi
 */

#include "I2C_via_GPIO.h"

void test_write();
void test_read();

int main(void)
{
    i2vg_init();

    test_wirte();
    test_read();
}

void test_write()
{
    i2vg_start();
    i2vg_write(0xc0);
    i2vg_write(0x40);
    i2vg_write(0xA0);
    i2vg_write(0x50);
    i2vg_stop();
    i2vg_write(0x40);
    i2vg_write(0xA0);
    i2vg_write(0x50);
    i2vg_stop();
}

void test_read()
{
    unsigned char data[16];

    i2vg_start();
    i2vg_write(0xc1);
    data[0] = i2vg_read();
    data[1] = i2vg_read();
    data[2] = i2vg_read();
    data[3] = i2vg_read();
    data[4] = i2vg_read();
    i2vg_stop();
}
