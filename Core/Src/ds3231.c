/*
 * @copyright Copyright (c) 2022
 * Course: Embedded Systems 2021/2022
 *
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Group:
 *
 * Ranieri Speranza 	0622701687	s.ranieri4@studenti.unisa.it
 * Mancusi Ernesto		0622701668	e.mancusi2@studenti.unisa.it
 * Parrella Giuseppe	0622701665	g.parrella9@studenti.unisa.it
 * Sonnessa Francesco	0622701672	f.sonnessa@studenti.unisa.it
 *
 * Copyright (C) 2022 - All Rights Reserved
 * This file is part of Air Mouse.
 *
 * Air Mouse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Air Mouse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Air Mouse.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <string.h>
#include "i2c.h"
#include "ds3231.h"

#define ADDRESS_SIZE				(1)
#define DATA_TRANSFER_SIZE			(7)

#define DS3231_ADDRESS 				(0xD0)
#define MAX_RETRY					(3)

#define DS3231_SECONDS				(0x00)
#define DS3231_MINUTES				(0x01)
#define DS3231_HOURS				(0x02)
#define DS3231_DAY					(0x03)
#define DS3231_DATE					(0x04)
#define DS3231_MONTH				(0x05)
#define DS3231_YEAR					(0x06)
#define DS3231_CONTROL				(0x07)

/* Bits in control register */
#define DS3231_CONTROL_OUT			(7)
#define DS3231_CONTROL_SQWE			(4)
#define DS3231_CONTROL_RS1			(1)
#define DS3231_CONTROL_RS0			(0)

I2C_HandleTypeDef* I2C_Port;

uint8_t months[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


/*
* @fn          static uint8_t bcd2Dec ( uint8_t val )
* @brief       Convert BCD to Decimal
* @param[in]   val value to convert from BCD to Decimal
* @return      res Converted value
*/
uint8_t bcd2Dec ( uint8_t val )
{
    uint8_t res = ((val/16*10) + (val % 16));
    return res;
}

/*
* @fn           static uint8_t dec2Bcd ( uint8_t val )
* @brief        Convert Decimal to BCD
* @param[in]    val value to convert from Decimal to BCD
* @return       res Converted value
*/
uint8_t dec2Bcd ( uint8_t val )
{
    uint8_t res = ((val/10*16) + (val%10));
    return res;
}


int8_t rtc_init(I2C_HandleTypeDef* I2Cx)
{
	I2C_Port = I2Cx;
	HAL_StatusTypeDef returnValue;
	returnValue = HAL_I2C_IsDeviceReady(I2C_Port, DS3231_ADDRESS, MAX_RETRY, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
	{
		return DS3231_I2C_ERR;
	}
	return DS3231_OK;
}

int8_t rtc_get_date_time(date_time_t* datetime)
{
	HAL_StatusTypeDef returnValue;
	uint8_t in_buff[DATA_TRANSFER_SIZE];
	memset(&in_buff,0,DATA_TRANSFER_SIZE);

	returnValue = HAL_I2C_Mem_Read(I2C_Port, DS3231_ADDRESS, DS3231_SECONDS, ADDRESS_SIZE, in_buff, DATA_TRANSFER_SIZE, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
	{
		return DS3231_I2C_ERR;
	}

	datetime->seconds = bcd2Dec(in_buff[0]);
	datetime->minutes = bcd2Dec(in_buff[1]);
	datetime->hours = bcd2Dec(in_buff[2]);
	datetime->day = bcd2Dec(in_buff[3]);
	datetime->date = bcd2Dec(in_buff[4]);
	datetime->month = bcd2Dec(in_buff[5]);
	datetime->year = bcd2Dec(in_buff[6]);

	return DS3231_OK;
}

int8_t rtc_set_date_time(const date_time_t* datetime)
{
	HAL_StatusTypeDef returnValue;
	uint8_t out_buff[DATA_TRANSFER_SIZE+ADDRESS_SIZE];

	//Putting the address
	out_buff[0] = DS3231_SECONDS;
	//Filling the data
	out_buff[1] = dec2Bcd(datetime->seconds);
	out_buff[2] = dec2Bcd(datetime->minutes);
	out_buff[3] = dec2Bcd(datetime->hours);
	out_buff[4] = dec2Bcd(datetime->day);
	out_buff[5] = dec2Bcd(datetime->date);
	out_buff[6] = dec2Bcd(datetime->month);
	out_buff[7] = dec2Bcd(datetime->year);


	returnValue = HAL_I2C_Mem_Write(I2C_Port, DS3231_ADDRESS,  DS3231_SECONDS, ADDRESS_SIZE, out_buff+1, DATA_TRANSFER_SIZE, HAL_MAX_DELAY);
	if(returnValue != HAL_OK)
	{
		return DS3231_I2C_ERR;
	}

	return DS3231_OK;

}

int8_t rtc_dispose(){
	return DS3231_OK;
}
