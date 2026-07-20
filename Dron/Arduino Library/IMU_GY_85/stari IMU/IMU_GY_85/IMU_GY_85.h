#include "Arduino.h"
#include <Wire.h>

#ifndef IMU_GY_85_h
#define IMU_GY_85_h

//-------------address--------------//
#define ADXL345 (0x53)              // Device address as specified in data sheet  // ADXL345 acceleromete 
#define DATAX0  (0x32)              //X-axsis Data 0
//#define DATAX1  0x33              //X-axsis Data 1
//#define DATAY0  0x34              // Y-axsis Data 0
//#define DATAY1  0x35              // Y-axsis Data 1
//#define DATAZ0  0x36              // Z-axsis Data 0
//#define DATAZ1  0x37              // Z-axsis Data 1
#define HMC5883  (0x1E)             //gyro
#define ITG3200  (0x68)             //compass
#define PI 3.14159265


class IMU_GY_85
{
public:
	IMU_GY_85();
	void GyroCalibrate();
	void SetGyro();
	void SetAccelerometer();
	


	
	void init();
	double* readAccelerometer();	// vrne vrednost v m^2/s
	double* readGyro();			//podatek vrne v deg/s


	//callback functions
	inline double accelerometer_x(double* a) { return *(a); }
	inline double accelerometer_y(double* a) { return *(1 + a); }
	inline double accelerometer_z(double* a) { return *(2 + a); }

	//-------------to je delal v drugem programu-------//

	//  event->acceleration.x = getX() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;     
	//  event->acceleration.y = getY() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;
	//  event->acceleration.z = getZ() * ADXL345_MG2G_MULTIPLIER * SENSORS_GRAVITY_STANDARD;

	//------------------------------------------------------

	inline double compass_x(double* a) { return *(a); }
	inline double compass_y(double* a) { return *(1 + a); }
	inline double compass_z(double* a) { return *(2 + a); }

	//------------------------------------------------------

	inline double gyro_x(double* a) { return *(a); }
	inline double gyro_y(double* a) { return *(1 + a); }
	inline double gyro_z(double* a) { return *(2 + a); }
	inline double temp(double* a) { return 35 + (*(3 + a) + 13200) / 280; }

	//---------------koti-----------------------------//
	inline double theta(double* a) { return *(a); }
	inline double phi(double* a) { return *(1 + a); }
	inline double psi(double* a) { return *(2 + a); }


	// funkcija za branje stanje pospeska in to pretvoriti v kot (nagnjenost)
	double* getKot();

	



};

#endif
