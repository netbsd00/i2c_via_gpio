/*
 * I2C_via_GPIO.h
 *
 *  Created on: 2023. 4. 3.
 *      Author: hwyi
 */

#ifndef I2C_via_GPIO_H
#define I2C_via_GPIO_H

#define ACK             0
#define NACK            1

typedef unsigned char   uint8_t;
typedef signed char     int8_t;
typedef unsigned short  uint16_t;
typedef signed short    int16_t;
typedef unsigned int    uint32_t;
typedef signed int      int32_t;

void i2vg_init(void);
uint8_t i2vg_write(uint8_t byte);
uint8_t i2vg_read(void);
void i2vg_start(void);
void i2vg_stop(void);
#endif //ifndef I2C_via_GPIO_H
