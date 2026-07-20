#ifndef INICIALIZACIJA_H
#define INICIALIZACIJA_H


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------ inicializacija spremenljivk: REGULACIJA PID
double pogon = 0;
double pogon_D = 10.0;    // vrednost za koliko premaknemo v levo, nato pri pretvorbi nazaj to tudi upostevamo
double pogon_vred = 0;  // to je cela stevilo, zamaknjeno v levo za x100
int levo, desno = 0;
/*
double kot_x_os = 0;
double kot_y_os = 0;
int kot = 0;
int jakost_odziva = 0;
*/
double data;    //prebrani podatek preko PC-ja
double korekcijaSZ_Yos = 0.8;
double korekcijaSZ_Yos_D = 1000.0;
double korekcijaSZ_Yos_vred = 8;  // vrednost brez decimalne vrednosti, le cisto stevilo
double korekcijaSZ_Yos_maks = 1.0;

double korekcijaSZ_Xos = 0.8;
double korekcijaSZ_Xos_D = 1000.0;
double korekcijaSZ_Xos_vred = 8;  // vrednost brez decimalne vrednosti, le cisto stevilo
double korekcijaSZ_Xos_maks = 5.0;

double pid_p_gain_roll = 0;
double pid_p_gain_D = 1000.0;
double pid_p_gain_vred = 0;
double pid_p_gain_maks = 10;

double pid_i_gain_roll = 0;
double pid_i_gain_D = 1000.0;
double pid_i_gain_vred = 0;
double pid_i_gain_maks = 5;

double pid_d_gain_roll = 0;
double pid_d_gain_D = 1000.0;
double pid_d_gain_vred = 0;
double pid_d_gain_maks = 5;

double pid_max_roll = 400;
double pid_max_D = 10.0;
double pid_max_vred = 4000;
double pid_max_maks = 400;

///spremenljivke za funkcijo potencM
double potencVred = 0;
double DekadRazred = 1;
double range = 400;
int izbor = 1;
//////////////// spremenljivke za 16 push button matrix \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// stolpci 4-:-7    // uporabljam le prvo vrstico ter S1, S2, S5 in S6 !!!!!
/*  NE DELA KER MORAM UPORABITI MISO& MOSI ZA ANTENO!!!!
int st1 = 11;
int st2 = 10;
int st3 = 10;
int st4 = 1;
//vrstice 4-:-7  // ko postavimo na LOW, se zazna pristisk gumba
int vr1 = 13;
int vr2 = 12;
int vr3 = 5;
int vr4 = 4;
*/
int tmp[17] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////// 16 Pushbutton matrix
int PUSHBUTTON(void);
int dovoljenjeBranje = 1; // 1-- dovoljeno, 0-- ni dovoljeno
int tipka = 0;            // tu bomo brali kaj dobimo iz tipke
int tipka_tmp = 1;        // za gumb- EN ZELEN GUMB, zacasna spremenljivka, da lahko ustvarim fronte
                          //////////// LCD\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                          void LCD_N(double vred_LCD_N[6]);
void LCD_S(char vred_LCD_C[], int stolpec, int vrstica);
void LCD_show(void);
int time_counter = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////Nastavljanje korekcijske faktorje \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void Koef_spreminjanje(void);
void displ(void);  //predstavi trenutne vrednosti in ponudi moznosti za naprej
void korekPr(void);
void pid_p(void);
void pid_i(void);
void pid_d(void);
void pid_max(void);
double branje(double j);     // branje Serial-PC

/*/////////////////////////////////  Branje potenciometra //////////////////////////////////////////////////*/
void potencRead(void);

/*//////////////////////////// ANTENA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void antena_send(void);

/*//////////////////////////// Stikalo izklopa pogona \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
int Switch_D_pin = 10;
int Izklop_pogona = 0;      // 0 - vse prav,  1 - onemogoci pogon
void Inicializacija_Turn_off_switch(void);
void Turn_off_swithc(void);


// vkljucimo se ostale zavihtke kode
#include "za_PUSHBUTTON.h"
#include "zab_KOEFICIENT_SPRE.h"
#include "zb_POTENCIMETER.h"
#include "zc_LCD.h"
#include "ze_STIKALO.h"
#include "zd_ANTENA.h"


#endif
