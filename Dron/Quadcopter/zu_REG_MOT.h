///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// REGULACIJSKE FUNKCIJE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void init_regulacija(void)
{ // ta funkcija mora biti inicializira po inicializaciji IMU-ja, tam prvic izmerimo&shranimo kote
  // nujno potreben, sicer bo na zacetku sunek
    yaw_angle.zeljena_vred = kot_merjeno.ZAxis;

    //Reset the PID controllers for a bumpless start.
    roll_angle.integral_sum = 0;
    roll_angle.last_error = 0;

    pitch_angle.integral_sum = 0;
    pitch_angle.last_error = 0;

    yaw_angle.integral_sum = 0;
    yaw_angle.last_error = 0;
};
//////////////////////////////////////////////////////////////////////

void preimenovanje(void)
{
    // ------ KOTI -> STABILIZACIJA -----------//
    // izmerjeni KOT
    roll_angle.izmerjena_vred = kot_korek.XAxis;
    pitch_angle.izmerjena_vred = kot_korek.YAxis;
    yaw_angle.izmerjena_vred = kot_korek.ZAxis;

    // zeljeni KOT
    roll_angle.zeljena_vred = 0;   // v trenutni regulaciji le pogon spreminjamo
    pitch_angle.zeljena_vred = 0;  // v trenutni regulaciji le pogon spreminjamo
    yaw_angle.zeljena_vred = 0;    // zeljen_kot_z + zavijanje_levo * 0.25 - zavijanje_desno * 0.25; //pozitivna smer vrtenja je CCW; s tem bomo dolocali zeljeno pozicijo vrtenja


    // ------ KOTNA HITROST -> RATE CONTROLER      COMPLEMENTARNI FILTER ---//
    kotna_hitrost_filt.XAxis = kotna_hitrost_filt.XAxis * compl_old + kotna_hitrost.XAxis* compl_new;
    kotna_hitrost_filt.YAxis = kotna_hitrost_filt.YAxis * compl_old + kotna_hitrost.YAxis* compl_new;
    kotna_hitrost_filt.ZAxis = kotna_hitrost_filt.ZAxis * compl_old + kotna_hitrost.ZAxis* compl_new;

    roll_rate.izmerjena_vred  = kotna_hitrost_filt.XAxis;
    pitch_rate.izmerjena_vred = kotna_hitrost_filt.YAxis;
    yaw_rate.izmerjena_vred   = kotna_hitrost_filt.ZAxis;

    // korekcija osi in zavijanje
    korekcijaSZ_Xos = tmp_korekcijaSZ_Xos;
    zavijanje_levo = 0;  // v trenutni regulaciji le pogon spreminjamo
    zavijanje_desno = 0;  // v trenutni regulaciji le pogon spreminjamo
};
/////////////////////////////////////////////////////////////////////////////

void izracun_PID_1velicina(struct PID *vel)
{
    //izracun pogreska od zeljene vrednosti
    float error = vel->zeljena_vred - vel->izmerjena_vred;

    // s tem onemogocimo integralski pobeg (ko dosezemo vrhno vrednost PID_output, da se integral ne povecuje in leze se bolj v nasicenje)																								  
    if (abs(abs(vel->PID_output) - vel->PID_output_max) > 5)
    {
        vel->integral_sum += vel->I_gain * error;
    }; //izracun Integralske napake(zajema vse prejsne vzorce)

       // omejimo integralski del:
    if (vel->integral_sum > vel->integral_sum_max)
    {
        vel->integral_sum = vel->integral_sum_max;
    }
    else if (vel->integral_sum < -vel->integral_sum_max)
    {
        vel->integral_sum = -vel->integral_sum_max;
    }

    vel->PID_output = vel->P_gain * error + vel->integral_sum + vel->D_gain * (error - vel->last_error);   //izracun celotne napake okoli x-osi, PID regulacija
    
    if (vel->PID_output > vel->PID_output_max)
    {
        vel->PID_output = vel->PID_output_max;
    }
    else if (vel->PID_output < -vel->PID_output_max)
    {
        vel->PID_output = -vel->PID_output_max;
    }

    vel->last_error = error;    //shranimo prejsno vrednost napake, to se uporabi pri D-clenu(gledamo spremembo)
};
/////////////////////////////////////////////////////

void izracun_PID(void) 
{
    // Zunanja regulacija -> STABILIZACIJA
    //---------okoli x-osi (Roll)--------
    //izracun_PID_1velicina(&roll_angle);
    roll_rate.zeljena_vred = 0; // roll_angle.PID_output;

    //--------okoli y-osi  (Pitch)-------
    //izracun_PID_1velicina(&pitch_angle);
    pitch_rate.zeljena_vred = 0;  // pitch_angle.PID_output;
    //--------------------------------------------//

    //--------------------------------------------//
    // NOTRANJE REGULACIJA -> rate controler
    //---------okoli x-osi (Roll)--------
    izracun_PID_1velicina(&roll_rate);

    //--------okoli y-osi  (Pitch)-------
    izracun_PID_1velicina(&pitch_rate);
    //--------------------------------------------//

    // Y-os regulator je zakomentiran v Napetost zavihku, drugacen reg., prehod med 0 <-> 360
};
/////////////////////////////////////////////////////

void reset_PID_1velicina(struct PID *vel)
{
    // resetiram izhodno vrednost, integral sum ter prejsno napako. za bumbless start
    vel->PID_output = 0;
    vel->integral_sum = 0;
    vel->last_error = 0;
}
/////////////////////////////////////////////////////

void reset_PID(void)
{ //postavimo na 0 vse vrednosti, v primeru ko pade pogon < Prag_izklop_reg, da tako pozabimo napako od prej!
    // STABILIZACIJA
    //okoli x-osi
    reset_PID_1velicina(&roll_angle);
    //okoli y-osi
    reset_PID_1velicina(&pitch_angle);
    //okoli z-osi
    reset_PID_1velicina(&yaw_angle);

    // RATE CONTROLER
    //okoli x-osi
    reset_PID_1velicina(&roll_rate);
    //okoli y-osi
    reset_PID_1velicina(&pitch_rate);
    //okoli z-osi
    reset_PID_1velicina(&yaw_rate);
}
/////////////////////////////////////////////////////

void regulacija_driver(void)
{
    stanje = 5;                  //preverjanje ce pride do prekoracitve casovne rezine da vemo kje je prislo do tezave
    if (Glaven_pogon > Prag_izklop_reg)
    {
        preimenovanje();
        izracun_PID();
    }
    else
    {
        reset_PID();
    }
};
////////////////////////// KONEC REGULACIJE  /////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
/////////////////////// SCHEDULER: REG+MOT  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//
void reg_mot_driver()
{
    KalmanYZ();
    //Napetost();              // merjenje napetosti in dolocevanje parametrov v odvisnosti kje se  nahajamo
    posodobitevRegulacije(0);  // na podlagi napetosti baterije spreminjamo koeficiente
    regulacija_driver();        
    motorji_driver();
    cas_M = TCNT0;             // traja max 100 od 110     
    // ko bos dodajal regulacijo okoli Z osi, lahko das kalman za X v antena rezino
    if (cas_M > cas_M_max) cas_M_max = cas_M;
    stanje = 0;
}
/*++++++++++++++++  REGULACIJA  +++++++++++++*/
rtos_task_t task_REG_MOT =
{
    reg_mot_driver,     // function
    0,                  // last_tick
};