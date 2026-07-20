#include "IMU_GY_85.h"

//  ce zelis dobiti rezultat accelerometra v G-jih: pomnozi dobljene podatke z: 3.9mG/LSB
// ce zelis dobiti rezultat gyrota v degr/sek, deli dobljene podatke z:14,375

// accelerometer: nekam dodaj se takojsno pretvorbo iz izmerjene vrednosti v m/s
// ali v readAccelerometer ali pa v getKot

void IMU_GY_85::SetAccelerometer()
{

	// Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
	Wire.beginTransmission(ADXL345);    // start transmission to device
	Wire.write(0x2D);				    //send register address  // Power Control Register
	Wire.write(0x08);					//send value to write
	Wire.endTransmission();				//end transmission

										// Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
	Wire.beginTransmission(ADXL345);		//start transmision to device
	Wire.write(0x31);						//send register address
	Wire.write(0x0B);						//send value to write: 00->+/- 2g,01->4g, 10->8g, 11-> 16g; 0x0B->1011--> zadnji dve 11-> +/-16g, prva 1-ka pove da imamo full resulution--> 3.9mG/LSB
	Wire.endTransmission();					//end transmission        

};

double* IMU_GY_85::readAccelerometer()  // vrne vrednost v m^2/s
{
	static double axis[3];
	int buff[6];

	Wire.beginTransmission(ADXL345);		// start transmission to device
	Wire.write(DATAX0);						//sends address to read from
	Wire.endTransmission();					//end transmission

	Wire.beginTransmission(ADXL345);		//start transmission to device
	Wire.requestFrom(ADXL345, 6);			//request 6 bytes from device

	uint8_t i = 0;
	while (Wire.available())				//device may send less than requested (abnormal)
	{
		buff[i] = Wire.read();				//receive a byte
		i++;
	}
	Wire.endTransmission();					//end transmission

	axis[0] = ( ((buff[1]) << 8) | buff[0] )* 0.00396025 * 9.81 - 0.81;	// buff[1] se premakne 8 bitov v levo in nato se sesteje z buff[0],
	axis[1] = ( ((buff[3]) << 8) | buff[2] )* 0.00396025 * 9.81 - 0.89;	//  => tako dobimo dva zdruzena podatka: npr: buff[1]=1111, buff[0]=> axis[0]=11110000
	axis[2] = ( ((buff[5]) << 8) | buff[4] )* 0.00396025 * 9.81 + 2.16;	// buff[2] in buff[3] hranita podatke za axis[1], buff[4] in buff[5] hranita podatke za axis[2]
											// ce zelimo dobiti rezultat accelerometra v G-jih, moramo temu rezultatu najprej odsteti offset, in to pomnoziti z 3.90625mG/LSB
	return axis;							// (axis[0]-offset[0])*3.90625mG/LSB
};




//-------------------------------------------------------------------------------------------------------

static double g_offx = 0;
static double g_offy = 0;
static double g_offz = 0;


void IMU_GY_85::SetGyro()
{
	Wire.beginTransmission(ITG3200);
	Wire.write(0x3E);
	Wire.write(0x00);
	Wire.endTransmission();

	Wire.beginTransmission(ITG3200);
	Wire.write(0x15);
	Wire.write(0x07);
	Wire.endTransmission();

	Wire.beginTransmission(ITG3200);
	Wire.write(0x16);
	Wire.write(0x1E);				// +/- 2000 dgrs/sec, 1KHz, 1E,19
	Wire.endTransmission();

	Wire.beginTransmission(ITG3200);
	Wire.write(0x17);
	Wire.write(0x00);
	Wire.endTransmission();

	delay(10);

	GyroCalibrate();
};

void IMU_GY_85::GyroCalibrate()
{
	static double tmpx = 0;
	static double tmpy = 0;
	static double tmpz = 0;

	g_offx = 0;
	g_offy = 0;
	g_offz = 0;

	for (uint8_t i = 0; i < 10; i++)  //take the measurement from 10 gyro probes and divide it from the current probe
	{
		delay(10);
		double* gp = readGyro();
		tmpx += *(gp);
		tmpy += *(++gp);
		tmpz += *(++gp);
	}
	g_offx = tmpx*  14.375 / 10;
	g_offy = tmpy* 14.375 / 10;
	g_offz = tmpz* 14.375 / 10;
};

double* IMU_GY_85::readGyro()  //podatek vrne v deg/s
{
	static double axis[4];

	Wire.beginTransmission(ITG3200);
	Wire.write(0x1B);
	Wire.endTransmission();

	Wire.beginTransmission(ITG3200);
	Wire.requestFrom(ITG3200, 8);			// request 8 bytes from ITG3200

	int i = 0;
	uint8_t buff[8];

	while (Wire.available())
	{
		buff[i] = Wire.read();
		i++;
	}
	Wire.endTransmission();
	// zacasno bom dal stran offset: - g_offx, - g_offy,  - g_offz
	//izmerjen offset za gyro:
	// x-os: -52.46875
	// yos: +30.7625
	// z-os: -2.0125
	axis[0] = ( ((buff[4] << 8) | buff[5]) - 52.46875) / 14.375;		// x-os
	axis[1] = ( ((buff[2] << 8) | buff[3]) + 30.7625) / 14.375;		// y-os
	axis[2] = ( ((buff[6] << 8) | buff[7]) - 2.0125) / 14.375;		// z-os
	axis[3] = ( ((buff[0] << 8) | buff[1]) + 13200  ) / 280 + 35;	// temperature

	return axis;
};

void IMU_GY_85::init()
{
	SetAccelerometer();
	SetGyro();

	

	
};

//____________dolocevanje kota___________

IMU_GY_85::IMU_GY_85(){				// inicializacija, nazacetku naredi, da definiras vse kote!!!

	static double theta = 0; //v smeri uringa kazalca okoli x-osi  (-)
	static double phi = 0;   // v smeri uringa kazalca okoli y-osi (|)
	static double psi = 0;   // v smeri uringa kazalca okoli z-osi   '|'  
							 // na listu imas oznacen kako sem definiral osi
};

double* IMU_GY_85::getKot()
{
	static double kot[3];
	static int tmpx = 0;
	static int tmpy = 0;
	static int tmpz = 0;

	double* ap = readAccelerometer();   // mogoce rabis se: IMU_GY_85::readAccelerometer()
	tmpx = *(ap);			//dodati moras pretvorbo v stopinje in offset
	tmpy = *(++ap);
	tmpz = *(++ap);

	kot[0] = atan2(tmpy, tmpz) * 180 / PI;   //kot vrnjen v stopinjah     // okoli x-sa
	// adapteran kot okoli y-na, preuredimo da bo s startne pozicije kot 0°
	kot[1] = atan2(tmpz, tmpx) * 180 / PI - 90;		//kot vrnjen v stopinjah  // okoli y-na
	if ((kot[1]<-180) && kot[1]>-271) {
		kot[1] = kot[1] + 360;
	};
	//dodana minusa pred tmpx, da dobimo kot ki narasca v pozitivni smeri vrtenja gyrota
	kot[2] = atan2(-tmpx, tmpy) * 180 / PI;		//kot vrnjen v stopinjah  // okoli z-ja

	return kot;

};
