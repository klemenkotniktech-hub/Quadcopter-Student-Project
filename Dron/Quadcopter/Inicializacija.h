#ifndef INICIALIZACIJA_H
#define INICIALIZACIJA_H

#define DECLINATION 0.068359
#define rad_to_deg 57.29578
#define deg_to_rad 0.017453292

// ---- Define struct --------------------------------------------------
struct XYZ
{
    float XAxis;
    float YAxis;
    float ZAxis;
};

struct PID
{
    float zeljena_vred;
    float izmerjena_vred;
    float P_gain;           //Gain setting for P-controller
    float I_gain;           //Gain setting for I-controller
    float D_gain;           //Gain setting for D-controller 
    float integral_sum;     // sestevek integralskega dela do tega trenutka
    float integral_sum_max; // omejitev integralskega dela
    float PID_output;       // output value of PID controler
    float PID_output_max;   //Maximum output of the PID-controller (+/-)
    float last_error;

    float tmp_P_gain;       //Gain setting for the roll P-controller
    float tmp_I_gain;       //Gain setting for the roll I-controller
    float tmp_D_gain;       //Gain setting for the roll D-controller

};

//----------- Struct variables -----------------------------------------
Kalmanf kalmanX;
Kalmanf kalmanY;
Kalmanf kalmanZ;

// struktura za IMU
IMU_GY_85 IMU;
DFRobot_QMC5883 compass;

Vector compass_komp, compass_komp_tmp = { 0,0,0};

//-----------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////
//-----------  RTOS  ---------------------------------------------------
int stanje_ERROR = 0;
static int start = 1;  //zacetek RTOS-a
static int LED = 3;    // ledica ki bo utripala v primeru napake
int flag_ERROR = 0;      // 0 -> vse uredu, 1 -> NAPAKA; PRENEHAJ Z VSEM!!!!
int flag_stanje = 0;     // ob napaki bom sem zapisal stanje (rezino) v kateri je prislo do napake moram imeti se eno spremenljivko, ker ko 
                         //  bo prislo do napake, bo sistem moral izstopiti iz dane rezine, tako da bo lahko ciklav v glavni kodi
int counter_goal = 0;    // to je zelena meja, ko counter_start pride do te vrednosti, se zgodi manuver -> LEDica
int flag_LED_uspeh = 0;     // 1-> uspelo, 0-> ni uspelo   zastava za counter_start = counter_goal   -> LED lahko naredi nekaj

int counter_perioda_LED, counter_rezina_LED = 0;    // rezina*rtos_num_task = perioda
void LED_counter_increase(void);
void Error_loop(void);
/////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//----------------------- KODA ZA: STANJE --------------------------------
// -------- spremenljivke ------------------------------------------------
XYZ kot_merjeno;             // izmerjeni koti z accelerometrom
XYZ kot_korek;               // popravljeni koti s pomoèjo kalmanovega filtra
XYZ gyro_offset;             // offsetna vrednost kotne hitrosti
XYZ kotna_hitrost;           // kotna hitrost izmerjena z gyrotom
XYZ kotna_hitrost_filt;      // tu se uporabi complementarni filter 
// to sta utezi za complementarni filter kotne hitrosti
const float compl_old = 0.4; // utez za stari del  } skupaj mora biti 1
const float compl_new = 0.6; // utez za novi del   }
float  dt = 0.0096851711;    // perioda se zgodi vsakih: 118,1 Hz 
                             // 110 taktov: 0.0088709675    115 taktov: 0.0092780693

// ------- Funkcije -------------------------------------------------------
//--------  IMU----------
void init_IMU(void);
void Gyro(void);
void Accelerometer(void);
void GyroCalibrate(int samples, int time_Delay_ms);
void Compass(void);
void Compass_Finish(void);

// --- Kalman filter
void init_Kalman(void); //postavitev zacetnih kotov
void KalmanX(void);

void KalmanYZ(void);

///////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------
//-------INICIALIZACIJA ANTENE---------------------------------------
int paket[32] = { 0,0,0,0,0,0,0 };   //ce ne opredelimo vrednosti, jih program sam--> lahko prevede do tezav
                                     //spremenljivke za avtomaticno popravo kvadrokopterja--> ko ne dobi vec casa nobene informacije
int antena_counter = 0;          //stevec koliko casa nismo prejeli nikakrsnega signala
int zacas_pogon = 0;                //vrednosti, ko dron ne prejema signala, da se ga vodi-->lebdenje / zaustavitev, tako da ne strmoglavi
int zacas_kot_x_os, zacas_kot_y_os = 0;

void init_antena(void);
float Glaven_pogon = 0;

float pogon_D = 10.0;
float SZ_Yos_D = 1000.0;
float SZ_Xos_D = 1000.0;
float pid_p_gain_D = 1000.0;
float pid_i_gain_D = 1000.0;
float pid_d_gain_D = 1000.0;
float pid_max_D = 10.0;
XYZ tmp_zeleni_kot;             // uporabnik pove kaksne kote zeli
int Izklop_pogona = 0;  // 0 -> pogon deluje, 1-> pogon onemogoèen
                        //////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------INICIALIZACIJA REGULACIJE---------------------------------------
void init_regulacija(void);
void izracun_PID_1velicina(struct PID *vel);
void reset_PID_1velicina(struct PID *vel);
//---------PID gain and limit settings--spremenljivke-----------------------
// spremenljivke za namen testiranja:
float current_error = 0;
float razlika_error = 0;

//------- okoli x-osi (Roll)---------
PID roll_rate =  // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    0,      // zeljen_kot   (kotna hitrost)
    0,      // izmerjen_kot (kotna hitrost)
    1,      // P_gain
    0.0,    // I_gain
    0,      // D_gain
    0,      // integral_sum
    50,     // integral_sum_max
    0,      // PID_output
    100,    // PID_output_max
    0,      // last_error

    0.5,    // tmp_P_gain
    0.0,    // tmp_I_gain
    1,      // tmp_D_gain
};

PID roll_angle =            // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    0,      // zeljen_kot   (kot)
    0,      // izmerjen_kot (kot)
    1,      // P_gain
    0.0,    // I_gain
    0,      // D_gain
    0,      // integral_sum
    50,     // integral_sum_max
    0,      // PID_output
    100,    // PID_output_max
    0,      // last_error

    0.5,    // tmp_P_gain
    0.0,    // tmp_I_gain
    1,      // tmp_D_gain
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//------ okoli y-osi (Pitch)                //PRI MENI NE BOSTA ISTI VREDNOSTI, KER BATERIJA NI ENAKOMERNO PRERAZPOREJENA CEZ X/Y-OS!!!
PID pitch_rate =            // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    roll_rate.zeljena_vred,    // zeljen_kot    (kotna hitrost)
    roll_rate.izmerjena_vred,  // izmerjen_kot  (kotna hitrost)
    roll_rate.P_gain,        // P_gain
    roll_rate.I_gain,        // I_gain
    roll_rate.D_gain,        // D_gain
    0,                  // integral_sum
    50,                 // integral_sum_max
    0,                  // PID_output
    100,                 // PID_output_max
    0,                  // last_error

    roll_rate.tmp_P_gain,    // tmp_P_gain
    roll_rate.tmp_I_gain,    // tmp_I_gain
    roll_rate.tmp_D_gain,    // tmp_D_gain
};


PID pitch_angle =            // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    roll_angle.zeljena_vred,    // zeljena_vred     (kot)
    roll_angle.izmerjena_vred,  // izmerjena_vred   (kot)
    roll_angle.P_gain,        // P_gain
    roll_angle.I_gain,        // I_gain
    roll_angle.D_gain,        // D_gain
    0,                  // integral_sum
    50,                 // integral_sum_max
    0,                  // PID_output
    100,                 // PID_output_max
    0,                  // last_error

    roll_angle.tmp_P_gain,    // tmp_P_gain
    roll_angle.tmp_I_gain,    // tmp_I_gain
    roll_angle.tmp_D_gain,    // tmp_D_gain
};

//------- okoli z-osi (Yaw)
PID yaw_rate =            // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    0,      // zeljena_vred     (kotna hitrost)
    0,      // izmerjena_vred   (kotna hitrost)
    0.5,    // P_gain
    0.0,    // I_gain
    1,      // D_gain
    0,      // integral_sum
    5,     // integral_sum_max
    0,      // PID_output
    15,     // PID_output_max
    0,      // last_error

    0.5,    // tmp_P_gain
    0.0,    // tmp_I_gain
    1,      // tmp_D_gain
};


PID yaw_angle =            // Za vsak razlicni napetostni nivo baterije, so potrebniumerjeni svoji koeficienti:
{
    0,      // zeljena_vred (kot)
    0,      // izmerjena_vred (kot)
    0.5,    // P_gain
    0.0,    // I_gain
    1,      // D_gain
    0,      // integral_sum
    5,     // integral_sum_max
    0,      // PID_output
    15,     // PID_output_max
    0,      // last_error

    0.5,    // tmp_P_gain
    0.0,    // tmp_I_gain
    1,      // tmp_D_gain
};



int Prag_izklop_reg = 100;      // 450;                 // dokler je pogon manjsi od tega pragu, regulacija ne dela
boolean auto_level = true;                 //Auto level on (true) or off (false)
float zavijanje_levo, zavijanje_desno = 0; //Za lazje sledenje kodi

                                           // korekcijski faktorji asimetricne oblike    //
                                           // !!!! s tem se se poigraj!!!!
                                           // korekcija pogona, v x-osi(tam kjer je baterija vzporedno) morajo motorji mal vec delat
float korekcijaSZ_Xos = 1.0;
float tmp_korekcijaSZ_Xos = 1.0;

//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------
//-------INICIALIZACIJA MOTORJI---------------------------------------
Servo Mot_SD;  //spredaj desno-(oranze)   esc1 -> Mot_SD
Servo Mot_ZD;  //zadaj desno (crn)        esc2 -> Mot_ZD
Servo Mot_ZL;  //zadaj levo (crn)         esc3 -> Mot_ZL
Servo Mot_SL;  //spredaj levo (oranzen)   esc4 -> Mot_SL

void init_motor(void);
void pretvorbaVrednosti(double val1, double val2, double val3, double val4);  // pretvorba vrednosti v pravilne intervale za vhodne podatke ESC-jov
float limit_val_Mot(float val_mot);
float val_Mot_SD, val_Mot_ZD, val_Mot_ZL, val_Mot_SL = 0;
float motor_counter = 0;   //meri kolikokrat smo sli skozi to casovno rezino--> uporaba na zacetku, da na motorje ne pride nikakrsen signal
                            //--> da se ESC ne sprogramira oz ne spremeni max range


///////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------
//-------FUNKCIJA NAPETOSTI IN HITROSTI----------------------------
void Napetost(void);
void Napetost_nivo1(void);
void Napetost_nivo2(void);
void Napetost_nivo3(void);
void Napetost_nivo4(void);
void Napetost_nivo5(void);
void Napetost_nivo6(void);
void posodobitevRegulacije(int select);
double napetost = 0;
int V_nivo = 1;  // npr napetost: 1-> 12.6V-:-12V; 2-> 12.05V-:-11.4V; 3-> 11.45V-:-10.8V; 4-> 10.85V-:-10.2V; 5-> 10.25-:-9.6V; 6->9.65V-:-9V
int H_nivo = 1;  // npr Glaven_pogon: 1-> 0-:-600,  2-> 600-:-1100, 3-> 1100-:-1600,  4-> 1600-:-2000
int S_nivo = 11;  // prva stevilka pove napetostni nivo, druga pa hitrostni nivo
                  /////////////////////////////////////////////////////////////////////////////////


////////   KALMAN FUNKCIJE  \\\\\\\\\\\\\\\//
/////////////  KALMANOV FILTER -> TU JE, KER IMA TA REZINA SE VELIKO PROSTEGA CASA \\\\\\\/
void KalmanYZ(void)
{
    // kALMANOV FILTER -> ne potrebuje predhodno filtreranega signala (npr z low pass filter, je odvec)
    kot_korek.YAxis = kalmanY.getAngle(kot_merjeno.YAxis, kotna_hitrost.YAxis, dt);
    kot_korek.ZAxis = kalmanZ.getAngle(kot_merjeno.ZAxis, kotna_hitrost.ZAxis, dt);
};
/////////////  KALMANOV FILTER -> TU JE, KER IMA TA REZINA SE VELIKO PROSTEGA CASA \\\\\\\/
void KalmanX(void)
{
    // kALMANOV FILTER -> ne potrebuje predhodno filtreranega signala (npr z low pass filter, je odvec)
    kot_korek.XAxis = kalmanX.getAngle(kot_merjeno.XAxis, kotna_hitrost.XAxis, dt);
};






// vkljucimo se ostale zavihtke kode (taske)
#include "RTOS.h"
#include "zs_STANJE1.h"
#include "zs_STANJE2.h"
#include "zs_STANJE3.h"
#include "zt_ANTENA.h"
#include "zv_MOTOR.h"
#include "zu_REG_MOT.h"
#include "zv_NAPETOST.h"

#endif