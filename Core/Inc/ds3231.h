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

#ifndef INC_DS3231_H_
#define INC_DS3231_H_


#define DS3231_OK 		(0)
#define DS3231_ERR		(-1)
#define DS3231_I2C_ERR	(-2)


struct date_time_s
{
	uint8_t     seconds;
	uint8_t     minutes;
	uint8_t     hours;
	uint8_t     day;
	uint8_t     date;
	uint8_t     month;
	uint16_t    year;
};

typedef struct date_time_s date_time_t;

int8_t rtc_init(I2C_HandleTypeDef* I2Cx);

int8_t rtc_get_date_time(date_time_t* datetime);

int8_t rtc_set_date_time(const date_time_t* datetime);

int8_t rtc_dispose();

#endif /* INC_DS3231_H_ */
