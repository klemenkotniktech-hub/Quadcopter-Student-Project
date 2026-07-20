#include <Wire.h>
#include <SPI.h>
#include "math.h"
#include <IMU_GY_85.h> 
#include <DFRobot_QMC5883_FAST.h>
#include <Kalmanf.h>
// moje funkcije dodaj v: C:\Users\Klemen\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.23\libraries

#include <Servo.h>
//// debuging ///
float stanje = 0; //0--> koncal, vse ostalo pomeni da smo znotraj ene rezine
double esc_vrednost_1, esc_vrednost_2, esc_vrednost_3, esc_vrednost_4 = 0;
double V1, V2, V3, V4 = 0;
int  cas_S1, cas_S2, cas_S3,cas_A, cas_A_M, cas_M = 0;
int cas_S1_max, cas_S2_max, cas_S3_max, cas_A_max, cas_A_M_max, cas_M_max = 0;
#include "Inicializacija.h"





/////////////////////////////////////////////////////////
//--------- SETUP FUNKCIJA --------------------------
void setup() {

    Serial.begin(9600);
    ///////init za casovno rezino: STANJE///////////////////////
    init_IMU();

    Serial.println("inicializacija IMU-ja uspesna");
    /////////init za casovno rezino: ANTENA/////////////////////
    init_antena();
    Serial.println("inicializacija ANTENE uspesna");
    ////////init za casovno rezino: REGULACIJA///////////////
    init_regulacija();
    Serial.println("inicializacija REGULACIJE uspesna");
    ////////init za casovno rezino: MOTORJI///////////////
    init_motor();
    Serial.println("inicializacija MOTORJI uspesna");
    //////////////signalizacija-blink led, ce smo prekoracili casovno rezino/////////
    pinMode(LED, OUTPUT);
    /////////////inicializacija RTOS-ja/////////////////////////

    if (rtos_init() != 0)
        while (1) { Serial.println("neuspesno nalozen RTOS"); };
    Serial.println("inicializacija RTOS-a uspesna");
    Serial.println("Konec setup-a");
    Serial.println("Sprozenje RTOS-a");
    rtos_enable();
    stanje = -1;
}


void loop() {
    /*  // CASOVNE REZINE MERJENJE TRAJANJA
    Serial.print("Rezina cas,  S1: ");
    Serial.print(cas_S1);
    Serial.print("  S2: ");
    Serial.print(cas_S2);
    Serial.print("  S3: ");
    Serial.print(cas_S3);
    Serial.print("  A: ");
    Serial.print(cas_A);
    //Serial.print("  A_M: ");
    //Serial.print(cas_A_M);
    Serial.print("  M: ");
    Serial.print(cas_M);
    
    Serial.print("  pogon: ");
    Serial.println((int)Glaven_pogon);
    */
    
    /*  // MAX CASOVNE REZINE MERJENJE TRAJANJA
    Serial.print("Rezina cas,  S1_max: ");
    Serial.print(cas_S1_max);
    Serial.print("  S2_max: ");
    Serial.print(cas_S2_max);
    Serial.print("  S3_max: ");
    Serial.print(cas_S3_max);
    Serial.print("  A_max: ");
    Serial.print(cas_A_max);
    Serial.print("  A_M_max: ");
    Serial.print(cas_A_M_max);
    Serial.print("  M_max: ");
    Serial.print(cas_M_max);

    Serial.print("  pogon: ");
    Serial.println((int)Glaven_pogon);
    */ 

    /*  // ACCELEROMETER -> KOTI    --> NA VSAKE TOLIKO UMERI ACCELEROMETER!!!
    Serial.print("merjena kota X:");
    Serial.print(kot_merjeno.XAxis);
    Serial.print(" Y:");
    Serial.print(kot_merjeno.YAxis);
    
    Serial.print(" Z:");
    Serial.println(kot_merjeno.ZAxis);
    */


    /*  // GYRO KOTNA HITROST       --> DELA!
    Serial.print("Vrtenje X:");
    Serial.print(kotna_hitrost.XAxis);
    Serial.print(" Y:  ");
    Serial.print(kotna_hitrost.YAxis);
    Serial.print("  Z: ");
    Serial.println(kotna_hitrost.ZAxis);
    */

    /*  // KALMAN FILTER-> KOTI     --> DELA!! X in Y
    Serial.print("popr_koti X:");
    Serial.print(kot_korek.XAxis);
    Serial.print(" Y:");
    Serial.println(kot_korek.YAxis);
    /*
    Serial.print(" Z:");
    Serial.println(kot_korek.ZAxis);
    */

    /*
    // Gledanje kaj dobimo prek antene:
    Serial.print("Pogon:");
    Serial.print(Glaven_pogon);
    Serial.print("  korekcijaSZ_Xos:");
    Serial.print(korekcijaSZ_Xos);
    Serial.print("  roll P_gain:");
    Serial.print(roll.tmp_P_gain);
    Serial.print(" roll i_gain:");
    Serial.print(roll.tmp_I_gain);
    Serial.print("  roll d_gain:");
    Serial.println(roll.tmp_D_gain);
    */

    /*// Test regulacije STABILNOSTI:
    Serial.print("Roll:   Error: ");
    Serial.print(current_error);
    Serial.print("  P_del: ");
    Serial.print(roll.P_gain * current_error);
    Serial.print("  Sum_Integ: ");
    Serial.print(roll.integral_sum);
    Serial.print("  D_del: ");
    Serial.print(roll.D_gain * razlika_error);
    Serial.print(" PID_out: ");
    Serial.println(roll.PID_output);
    */
    /* // Test RATE regulacije:
    Serial.print("compl filt- rate:   Xos:: ");
    Serial.println(kotna_hitrost_filt.XAxis);
    Serial.print("  Yos: ");
    Serial.print(kotna_hitrost_filt.YAxis);
    Serial.print("  Zos: ");
    Serial.println(kotna_hitrost_filt.ZAxis);
    */

    // TEST POGONA -> SE PRAVILNO ODZIVA, NA PRAVEM MOTORJU
    Serial.print("vrednosti val_esc:  SD:");
    Serial.print(val_Mot_SD);
    Serial.print("  ZD:");
    Serial.print(val_Mot_ZD);
    Serial.print("  ZL:");
    Serial.print(val_Mot_ZL);
    Serial.print("  SL:");
    Serial.print(val_Mot_SL);
    Serial.print("compl filt- rate:   Xos:: ");
    Serial.println(kotna_hitrost_filt.XAxis);






    /*  // Compass meritev
    Serial.print(" X-Com:  ");
    Serial.print(norm1.XAxis);
    Serial.print(" Y-Com:  ");
    Serial.println(norm1.YAxis);
    */
    /*
    Serial.print("izmerjen kot z:");
    Serial.println(kot_merjeno.ZAxis);
    */







    /*
    Serial.print("Napetost: ");
    Serial.println(napetost);
    */
    /*
    Serial.print("Motorji: SL: ");
    Serial.print(V4);
    Serial.print("  ZL:  ");
    Serial.print(V3);
    Serial.print("  ZD:  ");
    Serial.print(V2);
    Serial.print("  SD:");
    Serial.print(V1);
    Serial.print("  pid_output_roll:  ");
    Serial.print(pid_output_roll);
    Serial.print("  error pri roll-u:  ");
    Serial.println(pid_last_roll_d_error);
    */

    /*
    Serial.print("PID popravki:x:");
    Serial.print(pid_output_roll );
    Serial.print("  y:");
    Serial.print(pid_output_pitch);
    Serial.print("  Z:");
    Serial.println(pid_output_yaw);

    //preverjam ce koda deluje kot zelim
    Serial.println("koti");
    Serial.println(koti[0]);
    Serial.println(koti[1]);
    Serial.println(koti[2]);
    */




    /*
    Serial.println("PID napake");
    Serial.println(pid_output_roll);
    Serial.println(pid_output_pitch);
    Serial.println(pid_output_yaw);
    */
    /*
    Serial.print("      pogon:");
    Serial.println(paket[0]);
    */
    /*
    Serial.print("PID-regulacija: x-os:SL");
    Serial.print(pogonPID_Z_SL);
    Serial.print("    SD:");
    Serial.print(pogonPID_Z_SD);
    Serial.print("    ZL:");
    Serial.print(pogonPID_Z_ZL);
    Serial.print("    ZD:");
    Serial.print(pogonPID_Z_ZD);
    Serial.print("     pogon:");
    Serial.println(paket[0]);
    */
    /*
    Serial.println((int)pogon_ZL);
    Serial.print("razliika pogona:");
    Serial.println(razlika_pogona);
    */
    /*
    Serial.print("val vrednosti: 1:");
    Serial.print(val_esc1);
    Serial.print("     2:");
    Serial.print(val_esc2);
    Serial.print("     3:");
    Serial.print(val_esc3);
    Serial.print("     4:");
    Serial.print(val_esc4);
    Serial.print("     pogon:");
    Serial.println(paket[0]);
    */
};