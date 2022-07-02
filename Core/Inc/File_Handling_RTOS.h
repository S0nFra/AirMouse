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
#ifndef FILE_HANDLING_RTOS_H_
#define FILE_HANDLING_RTOS_H_

#include "fatfs.h"
#include "string.h"
#include "stdio.h"
#include "fatfs_sd.h"


/* mounts the sd card*/
FRESULT Mount_SD (const TCHAR* path);

/* unmounts the sd card*/
void Unmount_SD (const TCHAR* path);

/* Start node to be scanned (***also used as work area***) */
FRESULT Scan_SD (char* pat);

/* Only supports removing files from home directory. Directory remover to be added soon */
FRESULT Format_SD (void);

/* write the data to the file
 * @ name : is the path to the file*/
FRESULT Write_File (char *name, char *data);

/* read data from the file
 * @ name : is the path to the file*/
FRESULT Read_File (char *name);

/* creates the file, if it does not exists
 * @ name : is the path to the file*/
FRESULT Create_File (char *name);

/* Removes the file from the sd card
 * @ name : is the path to the file*/
FRESULT Remove_File (char *name);

/* creates a directory
 * @ name: is the path to the directory
 */
FRESULT Create_Dir (char *name);

/* checks the free space in the sd card*/
void Check_SD_Space (void);

/* updates the file. write pointer is set to the end of the file
 * @ name : is the path to the file
 */
FRESULT Update_File (char *name, char *data);




#endif /* FILE_HANDLING_RTOS_H_ */
