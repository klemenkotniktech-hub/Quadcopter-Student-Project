/////////////////// STANJE 1 -- FUNKCIJE INICIALIZACIJE+ MERJENJE KOTA DRONA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//----------------------SETUP FUNKCIJE-KODA------------------------------------
void init_IMU(void)
{
    Wire.begin();      //inicializacija I2C-ja
    delay(10);
    IMU.init(); //inicializacija accelerometra in gyrota
    delay(50);

    GyroCalibrate(50, 12);
    // inicializiramo trenutno vrednost kotne hitrosti -> se uporabi v regulatorju
   

    // inicializacija Compasa
    compass.Init_Compass();

    //moramo se nastaviti kalmanov filter na podlagi trenutne zacetne pozicije
    init_Kalman();

};

//------------Gyro calibrate ----------------------------//
void GyroCalibrate(int samples, int time_Delay_ms)
{
    static double tmpx = 0;
    static double tmpy = 0;
    static double tmpz = 0;


    gyro_offset.XAxis = 0;
    gyro_offset.YAxis = 0;
    gyro_offset.ZAxis = 0;

    for (uint8_t i = 0; i < samples; i++)
    {
        delay(time_Delay_ms);
        double* gp = IMU.readGyro();
        tmpx += *(gp);
        tmpy += *(++gp);
        tmpz += *(++gp);
    }
    gyro_offset.XAxis = tmpx / samples;
    gyro_offset.YAxis = tmpy / samples;
    gyro_offset.ZAxis = tmpz / samples;
};

// inicializacija zacetne kotne hitrosti
void init_rate(void)
{
    Gyro();
    kotna_hitrost_filt.XAxis = kotna_hitrost.XAxis;
    kotna_hitrost_filt.YAxis = kotna_hitrost.YAxis;
    kotna_hitrost_filt.ZAxis = kotna_hitrost.ZAxis;
}

// inicializacija zacetnih kotov Kalmanovega filtra:
void init_Kalman(void)
{
    Accelerometer(); // dobimo zacetne kote X- in Y-osi
    kalmanX.setAngle(kot_merjeno.XAxis);
    kalmanY.setAngle(kot_merjeno.YAxis);

    // korekcijskemu kotu dolocim vrednost, v init compass tega se ne uporabi
    kot_korek.XAxis = kot_merjeno.XAxis;
    kot_korek.YAxis = kot_merjeno.YAxis;

    Compass();  // dobimo zacetne kote Z-osi 
    // moram se predelat podatek, saj funkcija Compass ne vrne Z-kota, temvec le prebere meritev
    compass_komp.XAxis = compass_komp.XAxis;
    compass_komp.YAxis = compass_komp.YAxis;
    Compass_Finish();
    kalmanZ.setAngle(kot_merjeno.ZAxis);
};



//-----------------------------------------------------------------------------
//----------------------GLAVNE FUNKCIJE-KODA------------------------------------
//----------------------FUNKCIJA DOLOCITEV KOTOV -------------------
void Accelerometer(void)
{
    double* ap = IMU.getKot();  //prejme podatke trenutne lege
    kot_merjeno.XAxis = *(ap);
    kot_merjeno.YAxis = *(++ap);
}

///////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
////////-----SCHEDULER--STANJE 1-KODA-------------------------------------------------------------------------------------------------


void get_stanje1_driver(void)
{
    stanje = 1;         //preverjanje ce pride do prekoracitve casovne rezine da vemo kje je prislo do tezave
    Accelerometer();    // traja max 100 od 110
    cas_S1 = TCNT0;
    if (cas_S1 > cas_S1_max) cas_S1_max = cas_S1;
    stanje = 0;
};
/*+++++++++++++ STANJE +++++++++++++*/

rtos_task_t task_STANJE1 = {
    get_stanje1_driver,     // function
    0,                      // last_tick
};
