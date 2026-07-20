/////////////////////////////  STANJE 2 --- MERJENJE KOTA ZASUKA + PORACUNANJE PORPAVLJENEGA KOTA   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


//-------------------------- FUNKCIJA DOLOCITVE VRTENJA -------------
void Gyro(void)
{
    double* vrten_tmp = IMU.readGyro();
    kotna_hitrost.XAxis = (double)(*vrten_tmp)   - gyro_offset.XAxis; //x-os   //podatki v deg/sek
    kotna_hitrost.YAxis = (double)*(++vrten_tmp) - gyro_offset.YAxis; //y-os
    kotna_hitrost.ZAxis = (double)*(++vrten_tmp) - gyro_offset.ZAxis; //z-os
};

/////////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
////////-----SCHEDULER--STANJE 2-KODA-------------------------------------------------------------------------------------------------
void get_stanje2_driver(void) {

    stanje = 2;    //preverjanje ce pride do prekoracitve casovne rezine da vemo kje je prislo do tezave
    Gyro();        // traja max 90 od 110
    cas_S2 = TCNT0;
    if (cas_S2 > cas_S2_max) cas_S2_max = cas_S2;
    stanje = 0;
};
/*+++++++++++++ STANJE +++++++++++++*/

rtos_task_t task_STANJE2 = {
    get_stanje2_driver,     // function
    0,                      // last_tick
};