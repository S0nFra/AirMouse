/*
 * mpu6050.h
 *
 *  Created on: Jun 6, 2022
 *      Author: gparrella12
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

/* Default I2C clock */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK              400000            /*!< Default I2C clock speed */
#endif

#include "i2c.h"
#include "gpio.h"

/**
 * @brief  Data rates predefined constants
 */
#define MPU6050_DataRate_8KHz       0   /* Sample rate set to 8 kHz */
#define MPU6050_DataRate_4KHz       1   /* Sample rate set to 4 kHz */
#define MPU6050_DataRate_2KHz       3   /* Sample rate set to 2 kHz */
#define MPU6050_DataRate_1KHz       7   /* Sample rate set to 1 kHz */
#define MPU6050_DataRate_500Hz      15  /* Sample rate set to 500 Hz */
#define MPU6050_DataRate_250Hz      31  /* Sample rate set to 250 Hz */
#define MPU6050_DataRate_125Hz      63  /* Sample rate set to 125 Hz */
#define MPU6050_DataRate_100Hz      79  /* Sample rate set to 100 Hz */

/**
 * @brief  MPU6050 can have 2 different slave addresses, depends on it's input AD0 pin
 */
typedef enum  {
	MPU6050_Device_0 = 0x00, /*!< AD0 pin is set to low */
	MPU6050_Device_1 = 0x02  /*!< AD0 pin is set to high */
} MPU6050_Device;

/**
 * @brief  MPU6050 result enumeration
 */
typedef enum  {
	MPU6050_Result_Ok = 0x00,          /* Everything OK */
	MPU6050_Result_Error,              /* Unknown error */
	MPU6050_Result_DeviceNotConnected, /* There is no device with valid slave address */
	MPU6050_Result_DeviceInvalid       /* Connected device with address is not MPU6050 */
} MPU6050_Result;

/**
 * @brief  Parameters for accelerometer range
 */
typedef enum  {
	MPU6050_Accelerometer_2G = 0x00, /* Range is +- 2G */
	MPU6050_Accelerometer_4G = 0x01, /* Range is +- 4G */
	MPU6050_Accelerometer_8G = 0x02, /* Range is +- 8G */
	MPU6050_Accelerometer_16G = 0x03 /* Range is +- 16G */
} MPU6050_Accelerometer;

/**
 * @brief  Parameters for gyroscope range
 */
typedef enum {
	MPU6050_Gyroscope_250s = 0x00,  /* Range is +- 250 degrees/s */
	MPU6050_Gyroscope_500s = 0x01,  /* Range is +- 500 degrees/s */
	MPU6050_Gyroscope_1000s = 0x02, /* Range is +- 1000 degrees/s */
	MPU6050_Gyroscope_2000s = 0x03  /* Range is +- 2000 degrees/s */
} MPU6050_Gyroscope;


/**
 * @brief  Main MPU6050 structure
 */
typedef struct  {
	/* Private */
	uint8_t Address;         /* I2C address of device. */
	float Gyro_Mult;         /* Gyroscope corrector from raw data to "degrees/s". Only for private use */
	float Acce_Mult;         /* Accelerometer corrector from raw data to "g". Only for private use */
	/* Public */
	int16_t raw_accelerometer_X; /* Raw Accelerometer value X axis */
	int16_t raw_accelerometer_Y; /* Raw Accelerometer value Y axis */
	int16_t raw_accelerometer_Z; /* Raw Accelerometer value Z axis */
	int16_t raw_gyroscope_X;     /* Raw Gyroscope value X axis */
	int16_t raw_gyroscope_Y;     /* Raw Gyroscope value Y axis */
	int16_t raw_gyroscope_Z;     /* Raw Gyroscope value Z axis */
	float accelerometer_X; /* Accelerometer value X axis */
	float accelerometer_Y; /* Accelerometer value Y axis */
	float accelerometer_Z; /* Accelerometer value Z axis */
	float gyroscope_X; /* Gyroscope value X axis */
	float gyroscope_Y; /* Gyroscope value Y axis */
	float gyroscope_Z; /* Gyroscope value Z axis */
	float   Temperature;     /* Temperature in degrees */
	//I2C_HandleTypeDef* I2Cx;
} MPU6050;


/**
 * @brief  Initializes MPU6050 and I2C peripheral
 * @param  *DataStruct: Pointer to empty @ref MPU6050_t structure
 * @param  DeviceNumber: MPU6050 has one pin, AD0 which can be used to set address of device.
 *          This feature allows you to use 2 different sensors on the same board with same library.
 *          If you set AD0 pin to low, then this parameter should be SD_MPU6050_Device_0,
 *          but if AD0 pin is high, then you should use SD_MPU6050_Device_1
 *
 *          Parameter can be a value of @ref MPU6050_Device_t enumeration
 * @param  AccelerometerSensitivity: Set accelerometer sensitivity. This parameter can be a value of @ref MPU6050_Accelerometer_t enumeration
 * @param  GyroscopeSensitivity: Set gyroscope sensitivity. This parameter can be a value of @ref MPU6050_Gyroscope_t enumeration
 * @retval Initialization status:
 *            - MPU6050_Result_t: Everything OK
 *            - Other member: in other cases
 */
MPU6050_Result MPU6050_Init(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Device DeviceNumber, MPU6050_Accelerometer AccelerometerSensitivity, MPU6050_Gyroscope GyroscopeSensitivity);

/**
 * @brief  Sets gyroscope sensitivity
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure indicating MPU6050 device
 * @param  GyroscopeSensitivity: Gyro sensitivity value. This parameter can be a value of @ref MPU6050_Gyroscope_t enumeration
 * @retval Member of @ref MPU6050_Result_t enumeration
 */
MPU6050_Result MPU6050_SetGyroscope(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Gyroscope GyroscopeSensitivity);

/**
 * @brief  Sets accelerometer sensitivity
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure indicating MPU6050 device
 * @param  AccelerometerSensitivity: Gyro sensitivity value. This parameter can be a value of @ref MPU6050_Accelerometer_t enumeration
 * @retval Member of @ref MPU6050_Result_t enumeration
 */
MPU6050_Result MPU6050_SetAccelerometer(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, MPU6050_Accelerometer AccelerometerSensitivity);

/**
 * @brief  Sets output data rate
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure indicating MPU6050 device
 * @param  rate: Data rate value. An 8-bit value for prescaler value
 * @retval Member of @ref MPU6050_Result_t enumeration
 */
MPU6050_Result MPU6050_SetDataRate(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct, uint8_t rate);

/**
 * @brief  Reads accelerometer data from sensor
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 * @retval Member of @ref MPU6050_Result_t:
 *            - SD_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
MPU6050_Result MPU6050_ReadAccelerometer(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct);

/**
 * @brief  Reads gyroscope data from sensor
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 * @retval Member of @ref MPU6050_Result_t:
 *            - SD_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
MPU6050_Result MPU6050_ReadGyroscope(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct);

/**
 * @brief  Reads temperature data from sensor
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 * @retval Member of @ref MPU6050_Result_t:
 *            - SD_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
MPU6050_Result MPU6050_ReadTemperature(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct);

/**
 * @brief  Reads accelerometer, gyroscope and temperature data from sensor
 * @param  *DataStruct: Pointer to @ref MPU6050_t structure to store data to
 * @retval Member of @ref SD_MPU6050_Result_t:
 *            - SD_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
MPU6050_Result MPU6050_ReadAll(I2C_HandleTypeDef* I2Cx,MPU6050* DataStruct);

#endif /* INC_MPU6050_H_ */
