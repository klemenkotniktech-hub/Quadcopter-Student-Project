#include "Kalmanf.h"

Kalmanf::Kalmanf() {
	// We will set the variables like so, these can also be tuned by the user// vsak tak objekt bo za skupaj za en kot in njegov zasuk(od gyrota)  
	Q_angle = 0.001f;
	Q_bias = 0.003f;
	R_measure = 0.03f;

	angle = 0.0f;	// reset the angle
	bias = 0.0f;	// reset bias

	P[0][0] = 0.0f;	// Since we assume that bias is 0 and we know the starting angle (use setAngle), the error covarince matrix is set like so: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
	P[0][1] = 0.0f;
	P[1][0] = 0.0f;
	P[1][1] = 0.0f;

	S = 0.0f;
	K[0] = 0.0f;
	K[1] = 0.0f;
};

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
float Kalmanf::getAngle(float newAngle, float newRate, float dt) {
	// KasBot V2 - Kalman filter module - http://www.x-firm.com/?page_id=145
	// Modified by Kristian Lauszus->   blog for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

	// Discrete Kalman filter time update equations - Time Update ("Predict")
	// Update xhat- Project the state ahead
	/* Step 1 */
	rate = newRate - bias;
	angle += dt * rate;

	// Update estimation error covariance - Project the error covariance ahead
	/* Step 2 */
	P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
	P[0][1] -= dt * P[1][1];
	P[1][0] -= dt * P[1][1];
	P[1][1] += dt * Q_bias;

	// Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
	// Kalculate Kalman gain - Compute the Kalamn gain
	/* Step 4 */
	S = P[0][0] + R_measure; // Estimate error
	
	/* Step 5 */
	 // Kalman gain - This is a 2x1 vector// tezava pri deljenju z majhnim stevilom--> ovf ali NaN
	K[0] = P[0][0] / S;
	K[1] = P[1][0] / S;
	
	// Calculate angle and bias - Update estimate with measurement zk (newAngle)
	/* Step 3 */
	float y = newAngle - angle; // Angle difference
	/* Step 6 */
	angle += K[0] * y;
	bias += K[1] * y;

	// Calculate estimation error covariance - Update the error covariance
	/* Step 7 */
	float P00_temp = P[0][0]; // zacasna spremenljivka-temp
	float P01_temp = P[0][1]; // zacasna spremenljivka-temp

	P[0][0] -= K[0] * P00_temp;
	P[0][1] -= K[0] * P01_temp;
	P[1][0] -= K[1] * P00_temp;
	P[1][1] -= K[1] = P01_temp;

	return angle;
};

/*---------zacetne nastavitve-------*/
void Kalmanf::setAngle(float angle) { this->angle = angle; };	// Used to set angle, this should be set as the starting angle
float Kalmanf::getRate() { return this->rate; };				// Return the unbiased rate

/* These are used to tune the Kalman filter, nastavljanje lastnosti filtra */
void Kalmanf::setQangle(float Q_angle) { this->Q_angle = Q_angle; };
void Kalmanf::setQbias(float Q_bias) { this->Q_bias = Q_bias; };
void Kalmanf::setRmeasure(float R_measure) { this->R_measure = R_measure; };


float Kalmanf::getQangle() { return this->Q_angle; };
float Kalmanf::getQbias() { return this->Q_bias; };
float Kalmanf::getRmeasure() { return this->R_measure; };

float Kalmanf::getEstimateError() { return this->S; };
float Kalmanf::getKalmangain0() { return this->K[0]; };
float Kalmanf::getKalmangain1() { return this->K[1]; };