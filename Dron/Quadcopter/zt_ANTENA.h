//#include "nRF24L01.h"
//#include "RF24.h"
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);                               //izbrani vhodni pin na arduinu
                                                //izberemo na katerem kanalu se bomo nahajali
const byte rxAddr[6] = { 00001 };                 // ce tole ne dela, spremeni nazaj v "00001"

                                                  //----inicializacija klicane funkcije
void antena_driver(void); // v SETUP funkciji moras dodati inicializacijo SPREJEMANJA PODATKOV!!!
                          /*+++++++++++++ ANTENA +++++++++++++*/
rtos_task_t task_ANTENA = {
    antena_driver,  // function
    0,              // last_tick
};
////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//----------------- INICIALIZACIJA FUNKCIJE SETUP ANTENE------------------------------------------------------

void init_antena(void) {
    radio.begin();
    radio.openReadingPipe(0, rxAddr);
    radio.startListening();
};
//------------------------------------------------------------------------------------------------------------
//--------------- DELOVANJE FUNKCIJE: antena_driver ----------------------------------------------------------
void antena_driver(void) {

    ////////////////////  COMPASS       \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
    Compass_Finish();
    KalmanX();
    // do tu porabi 40 od vsega 115 casa
    ////////////////////////////////////////////////////////////////////////////////////////////////


    stanje = 4;    //preverjanje ce pride do prekoracitve casovne rezine da vemo kje je prislo do tezave

    double tmp_preverjanje = 0;
    int uspeh = 1; // NE-1, JA-0
    int cas_konec = 1; // ce je urin counter>cas_meja-->cas_konec = 0, while funkcija se zakljuci
    int cas_meja = 70; // do koliko lahko pride counter pri casovnem interruptu, preden se bo ta ponovno sprozil,

    int zacasen_paket[32] = { 0,0,0,0,0,0,0 };
    //ko je while=1, bo funkcija delala
    while (cas_konec && uspeh) { //preverimo, ce je uspesno branje ali ce je premalo casa na razpolago-->TCNT0
        if (radio.available())
        {
            cas_A_M = TCNT0;
            //podatkovni vrstni red za RATE CONTR: pogon_vred; korekcijaSZ_Xos_vred; pid_p_gain_vred;pid_i_gain_vred;
            //podatki nadaljevanje:  pid_d_gain_vred; Izklop_pogona!!!
            radio.read(&zacasen_paket, sizeof(zacasen_paket));    //shranimo v zacasno spremenljivko, da ne pokvarimo prejsne meritve v primeru neuspeha
            if (zacasen_paket[0] || zacasen_paket[1] || zacasen_paket[2] || zacasen_paket[3] || zacasen_paket[4] || zacasen_paket[5]) { uspeh = 0; }; //preverimo, ce je bil uspesen prenos podatkov
            
            cas_A_M = TCNT0 - cas_A_M;  // traja max 10
            if (cas_A_M > cas_A_M_max) cas_A_M_max = cas_A_M;

        }
        if (TCNT0>cas_meja) { cas_konec = 0; };  //preverimo ce je premalo casa se za branje z antene
    };

    if (uspeh == 0) {//ce je branje uspesno, si zapolnimo vrednost
        int i_st = 0;
        for (i_st = 0; i_st < 6; i_st++) {
            paket[i_st] = zacasen_paket[i_st];    //prepisemo vrednosti, saj so te sedaj veljavne, sicer bi lahko dobili same 0-le na izhodu
        };
        antena_counter = 0;    //resetiramo stevec, ker smo prejeli signal
                               ////sedaj preoblikujemo podatke za PID-regulacijo:
                               // za pogon se spreminja v Regulaciji in v MOTORJI
        Glaven_pogon = (double)paket[0] / pogon_D;  //sprejeti podatek preko antene-->moc pogona//podatek pride vse do 1800
        tmp_korekcijaSZ_Xos = (double)paket[1] / SZ_Xos_D;
        roll_rate.tmp_P_gain = (double)paket[2] / pid_p_gain_D;
        roll_rate.tmp_I_gain = (double)paket[3] / pid_i_gain_D;
        roll_rate.tmp_D_gain = (double)paket[4] / pid_d_gain_D;
        Izklop_pogona = paket[5];
        // S_nivo = paket[6];  //dodaj, vrednost naj pride kot: 14 ->   (napetost= 1, hitrost=4)

        ////////se popravimo okoli Y-osi
        pitch_rate.tmp_P_gain = roll_rate.tmp_P_gain;  //Gain setting for the pitch P-controller.
        pitch_rate.tmp_I_gain = roll_rate.tmp_I_gain;  //Gain setting for the pitch I-controller.
        pitch_rate.tmp_D_gain = roll_rate.tmp_D_gain;  //Gain setting for the pitch D-controller.

                                             ///////// Dodati moram se zmoznost nastavljanja okoli Z-osi
    }
    else {
        /*

        if(antena_counter < 25) {antena_counter++;};

        if( (antena_counter > 10)){  //ce 1/5  sekund nobenega signala,

        //sedaj zelimo narediti avtomaticno stabilizacijo, da se vrne v vodoravno lego--> trust bom naknadno bolje dolocil
        // v 10 ciklih zelimo dron postaviti v vodoravno lego
        if(antena_counter == 10){   // prepisemo vrednosti pred 10-mi cikli
        zacas_pogon = paket[0];     //prepisemo vrednosti zeljenega pogona pred 10-mi cikli
        zacas_kot_x_os = paket[1];  //prepisemo vrednosti zeljenega x-kota pred 10-mi cikli
        zacas_kot_y_os = paket[2];  //prepisemo vrednosti zeljenega y-kota pred 10-mi cikli
        paket[3] = 0;               // prekinemo zavijanje levo
        paket[4] = 0;               // prekinemo zavijanje desno
        };

        if(paket[0] > 1200){// stabiliziramo dron na lebdenje v zraku
        paket[0] = zacas_pogon + (1300 - zacas_pogon)/15.0*(antena_counter - 10);  //po 15-ih korakih se gibamo do nevtralne sredinske tocke
        paket[1] = zacas_kot_x_os - zacas_kot_x_os/15.0*(antena_counter - 10);     //po 15-ih korakih se gibamo k vrednosti 0
        paket[2] = zacas_kot_y_os - zacas_kot_y_os/15.0*(antena_counter - 10);     //po 15-ih korakih se gibamo k vrednosti 0
        // po desetih ciklih bo zeljena vrednost na vodoravnem nivoju!!!
        }else {

        paket[0] = zacas_pogon - (zacas_pogon)/15.0*(antena_counter - 10);  //po 15-ih korakih se gibamo do nevtralne sredinske tocke
        paket[1] = zacas_kot_x_os - zacas_kot_x_os/15.0*(antena_counter - 10);     //po 15-ih korakih se gibamo k vrednosti 0
        paket[2] = zacas_kot_y_os - zacas_kot_y_os/15.0*(antena_counter - 10);     //po 15-ih korakih se gibamo k vrednosti
        }


        }
        if(paket[0] < 80){paket[0] = 0;};  //tezave s nekontroliranim pulznim zasuki
        */

    };              // traja max 92 od 110
    cas_A = TCNT0;
    if (cas_A > cas_A_max) cas_A_max = cas_A;
    stanje = 0;
};