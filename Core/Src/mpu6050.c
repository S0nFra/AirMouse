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

#include "mpu6050.h"

/* Default I2C address */
#define MPU6050_I2C_ADDR			0xD0

/* Who I am register value */
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO			0x01
#define MPU6050_SMPLRT_DIV			0x19
#define MPU6050_CONFIG				0x1A
#define MPU6050_GYRO_CONFIG			0x1B
#define MPU6050_ACCEL_CONFIG		0x1C
#define MPU6050_MOTION_THRESH		0x1F
#define MPU6050_INT_PIN_CFG			0x37
#define MPU6050_INT_ENABLE			0x38
#define MPU6050_INT_STATUS			0x3A
#define MPU6050_ACCEL_XOUT_H		0x3B
#define MPU6050_ACCEL_XOUT_L		0x3C
#define MPU6050_ACCEL_YOUT_H		0x3D
#define MPU6050_ACCEL_YOUT_L		0x3E
#define MPU6050_ACCEL_ZOUT_H		0x3F
#define MPU6050_ACCEL_ZOUT_L		0x40
#define MPU6050_TEMP_OUT_H			0x41
#define MPU6050_TEMP_OUT_L			0x42
#define MPU6050_GYRO_XOUT_H			0x43
#define MPU6050_GYRO_XOUT_L			0x44
#define MPU6050_GYRO_YOUT_H			0x45
#define MPU6050_GYRO_YOUT_L			0x46
#define MPU6050_GYRO_ZOUT_H			0x47
#define MPU6050_GYRO_ZOUT_L			0x48
#define MPU6050_MOT_DETECT_STATUS	0x61
#define MPU6050_SIGNAL_PATH_RESET	0x68
#define MPU6050_MOT_DETECT_CTRL		0x69
#define MPU6050_USER_CTRL			0x6A
#define MPU6050_PWR_MGMT_1			0x6B
#define MPU6050_PWR_MGMT_2			0x6C
#define MPU6050_FIFO_COUNTH			0x72
#define MPU6050_FIFO_COUNTL			0x73
#define MPU6050_FIFO_R_W			0x74
#define MPU6050_WHO_AM_I			0x75

/* Gyro sensitivities in degrees/s */
#define MPU6050_GYRO_SENS_250		((float) 131)
#define MPU6050_GYRO_SENS_500		((float) 65.5)
#define MPU6050_GYRO_SENS_1000		((float) 32.8)
#define MPU6050_GYRO_SENS_2000		((float) 16.4)

/* Acce sensitivities in g/s */
#define MPU6050_ACCE_SENS_2			((float) 16384)
#define MPU6050_ACCE_SENS_4			((float) 8192)
#define MPU6050_ACCE_SENS_8			((float) 4096)
#define MPU6050_ACCE_SENS_16		((float) 2048)

MPU6050_Result MPU6050_Init(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Device DeviceNumber, MPU6050_Accelerometer AccelerometerSensitivity, MPU6050_Gyroscope GyroscopeSensitivity)
{
	uint8_t WHO_AM_I = (uint8_t)MPU6050_WHO_AM_I;
	uint8_t temp;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t d[2];


	/* Format I2C address */
	DataStruct->Address = MPU6050_I2C_ADDR | (uint8_t)DeviceNumber;
	uint8_t address = DataStruct->Address;

	/* Check if device is connected */
	if(HAL_I2C_IsDeviceReady(Handle,address,2,5)!=HAL_OK)
	{
				return MPU6050_Result_Error;
	}
	/* Check who am I */
	//------------------
		/* Send address */
		if(HAL_I2C_Master_Transmit(Handle, address, &WHO_AM_I, 1, 1000) != HAL_OK)
		{
			return MPU6050_Result_Error;
		}

		/* Receive multiple byte */
		if(HAL_I2C_Master_Receive(Handle, address, &temp, 1, 1000) != HAL_OK)
		{
			return MPU6050_Result_Error;
		}

		/* Checking */
		while(temp != MPU6050_I_AM)
		{
				/* Return error */
				return MPU6050_Result_DeviceInvalid;
		}
	//------------------

	/* Wakeup MPU6050 */
	//------------------
		/* Format array to send */
		d[0] = MPU6050_PWR_MGMT_1;
		d[1] = 0x00;

		/* Try to transmit via I2C */
		if(HAL_I2C_Master_Transmit(Handle,(uint16_t)address , (uint8_t *)d, 2, 1000) != HAL_OK)
		{
					return MPU6050_Result_Error;
		}
	//------------------

	/* Set sample rate to 1kHz */
	MPU6050_SetDataRate(I2Cx,DataStruct, MPU6050_DataRate_1KHz);

	/* Config accelerometer */
	MPU6050_SetAccelerometer(I2Cx,DataStruct, AccelerometerSensitivity);

	/* Config Gyroscope */
	MPU6050_SetGyroscope(I2Cx,DataStruct, GyroscopeSensitivity);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_SetDataRate(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, uint8_t rate)
{
	uint8_t d[2];
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;
	/* Format array to send */
	d[0] = MPU6050_SMPLRT_DIV;
	d[1] = rate;

	/* Set data sample rate */
	while(HAL_I2C_Master_Transmit(Handle,(uint16_t)address,(uint8_t *)d,2,1000)!=HAL_OK);

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_SetAccelerometer(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Accelerometer AccelerometerSensitivity)
{
	uint8_t temp;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;
	uint8_t regAdd =(uint8_t )MPU6050_ACCEL_CONFIG;

	/* Config accelerometer */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address,&regAdd, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, &temp, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	temp = (temp & 0xE7) | (uint8_t)AccelerometerSensitivity << 3;
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address,&temp, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/

	/* Set sensitivities for multiplying gyro and accelerometer data */
	switch (AccelerometerSensitivity) {
		case MPU6050_Accelerometer_2G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_2;
			break;
		case MPU6050_Accelerometer_4G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_4;
			break;
		case MPU6050_Accelerometer_8G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_8;
			break;
		case MPU6050_Accelerometer_16G:
			DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_16;
			break;
		default:
			break;
		}

	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_SetGyroscope(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Gyroscope GyroscopeSensitivity)
{
	uint8_t temp;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;
	uint8_t regAdd =(uint8_t )MPU6050_GYRO_CONFIG;

	/* Config gyroscope */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address,&regAdd, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, &temp, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/
	temp = (temp & 0xE7) | (uint8_t)GyroscopeSensitivity << 3;
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address,&temp, 1, 1000) != HAL_OK);
	/*{
				return SD_MPU6050_Result_Error;
	}*/

	switch (GyroscopeSensitivity) {
			case MPU6050_Gyroscope_250s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
				break;
			case MPU6050_Gyroscope_500s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_500;
				break;
			case MPU6050_Gyroscope_1000s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_1000;
				break;
			case MPU6050_Gyroscope_2000s:
				DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_2000;
				break;
			default:
				break;
		}
	/* Return OK */
	return MPU6050_Result_Ok;
}

MPU6050_Result MPU6050_ReadAccelerometer(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct)
{
	uint8_t data[6];
	uint8_t reg = MPU6050_ACCEL_XOUT_H;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;

	/* Read accelerometer data */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address, &reg, 1, 1000) != HAL_OK);

	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, data, 6, 1000) != HAL_OK);

	/* Format raw data */
	DataStruct->raw_accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);
	/* Save original data */
	DataStruct->accelerometer_X = DataStruct->raw_accelerometer_X * DataStruct -> Acce_Mult;
	DataStruct->accelerometer_Y = DataStruct->raw_accelerometer_Y * DataStruct -> Acce_Mult;
	DataStruct->accelerometer_Z = DataStruct->raw_accelerometer_Z * DataStruct -> Acce_Mult;
	/* Return OK */
	return MPU6050_Result_Ok;
}
MPU6050_Result MPU6050_ReadGyroscope(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct)
{
	uint8_t data[6];
	uint8_t reg = MPU6050_GYRO_XOUT_H;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;

	/* Read gyroscope data */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address, &reg, 1, 1000) != HAL_OK);

	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, data, 6, 1000) != HAL_OK);

	/* Format raw data */
	DataStruct->raw_gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);
	/* Save original data */
	DataStruct->gyroscope_X = DataStruct->raw_gyroscope_X * DataStruct -> Gyro_Mult;
	DataStruct->gyroscope_Y = DataStruct->raw_gyroscope_Y * DataStruct -> Gyro_Mult;
	DataStruct->gyroscope_Z = DataStruct->raw_gyroscope_Z * DataStruct -> Gyro_Mult;
	/* Return OK */
	return MPU6050_Result_Ok;
}
MPU6050_Result MPU6050_ReadTemperature(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct)
{
	uint8_t data[2];
	int16_t temp;
	uint8_t reg = MPU6050_TEMP_OUT_H;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;

	/* Read temperature */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address, &reg, 1, 1000) != HAL_OK);

	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, data, 2, 1000) != HAL_OK);

	/* Format temperature */
	temp = (data[0] << 8 | data[1]);
	DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);

	/* Return OK */
	return MPU6050_Result_Ok;
}
MPU6050_Result MPU6050_ReadAll(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct)
{
	uint8_t data[14];
	int16_t temp;
	uint8_t reg = MPU6050_ACCEL_XOUT_H;
	I2C_HandleTypeDef* Handle = I2Cx;
	uint8_t address = DataStruct->Address;

	/* Read full raw data, 14bytes */
	while(HAL_I2C_Master_Transmit(Handle, (uint16_t)address, &reg, 1, 1000) != HAL_OK);

	while(HAL_I2C_Master_Receive(Handle, (uint16_t)address, data, 14, 1000) != HAL_OK);

	/* Format accelerometer data */
	DataStruct->raw_accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	DataStruct->raw_accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	DataStruct->raw_accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);
	/* Save original data */
	DataStruct->accelerometer_X = DataStruct->raw_accelerometer_X * DataStruct -> Acce_Mult;
	DataStruct->accelerometer_Y = DataStruct->raw_accelerometer_Y * DataStruct -> Acce_Mult;
	DataStruct->accelerometer_Z = DataStruct->raw_accelerometer_Z * DataStruct -> Acce_Mult;

	/* Format temperature */
	temp = (data[6] << 8 | data[7]);
	DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

	/* Format gyroscope data */
	DataStruct->raw_gyroscope_X = (int16_t)(data[8] << 8 | data[9]);
	DataStruct->raw_gyroscope_Y = (int16_t)(data[10] << 8 | data[11]);
	DataStruct->raw_gyroscope_Z = (int16_t)(data[12] << 8 | data[13]);
	/* Save original data */
	DataStruct->gyroscope_X = DataStruct->raw_gyroscope_X * DataStruct -> Gyro_Mult;
	DataStruct->gyroscope_Y = DataStruct->raw_gyroscope_Y * DataStruct -> Gyro_Mult;
	DataStruct->gyroscope_Z = DataStruct->raw_gyroscope_Z * DataStruct -> Gyro_Mult;

	/* Return OK */
	return MPU6050_Result_Ok;
}
