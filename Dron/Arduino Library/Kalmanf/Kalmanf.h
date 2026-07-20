#ifndef _Kalmanf_h_
#define _Kalmanf_h_

class Kalmanf {
public:
	Kalmanf();
	
	//The angle should be in degrees and the rate  should be in degrees per second and the delta time in seconds
	float getAngle(float newAngle, float newRate, float dt);
	
	void setAngle(float angle); // used to set angle, this should be set as the starting angle
	float getRate();			// return the unbiased rate

	/* these are used to tune the Kalman filter */
	void setQangle(float Q_angle);
	void setQbias(float Q_bias);
	void setRmeasure(float R_measure);

	/*vrednosti, katere preveris delovanje*/
	float getQangle();
	float getQbias();
	float getRmeasure();

	float getEstimateError();
	float getKalmangain0();
	float getKalmangain1();

private:
	/* Kalman filter variables */
	float Q_angle;	// Process noise variance for the accelerometer
	float Q_bias;	// Process noise variance  for the gyro bias
	float R_measure;// Measurement noise variance -this is actually the variance of the measurement noise

	float angle;	// The angle calculated by the Kalman filter- part of the 2x1 state vector
	float bias;		// The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
	float rate;		// Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

	float P[2][2];	// Error covariance matrix - This is a 2x2 matrix

	float S;
	float K[2];
};

#endif
