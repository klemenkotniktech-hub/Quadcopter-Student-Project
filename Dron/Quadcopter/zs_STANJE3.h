/////////////////////////////  STANJE 3 --- MERJENJE COMPASA IN POPRAVEK KOTA OKOLI Z-OSI   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//------------------- FUNKCIJA DOLOCITVE KOTA OKOLI Z-OSI ---------

void Compass(void)
{
    compass_komp_tmp = compass.readNormalizeXYZ();  // --> tukaj moras predelat funkcijo, 10prebere vrednosti in je predolga!!
    
    // nadaljni izracun se zgodi v rezini ANTENA TER REG&MOT
    // to pocnem z namenom porazdelitve obremenilnega casa posamezne rezine :)
    // korekcija X in Y komponente delam po metodi: IZRACUN in KOREKCIJA COMPASSA znotraj compass mape
    // uporabljam korekcijske kote, ki so za eno periodo zakasnjeni -> v compassu je ze veliko napake, to vpliva neki
    float tmp_pitch = kot_korek.YAxis* deg_to_rad;
    compass_komp.XAxis = compass_komp_tmp.XAxis * cos(tmp_pitch) + compass_komp_tmp.ZAxis * sin(tmp_pitch);
    
}

////////////////////  COMPASS       \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void Compass_Finish(void)   // -> to funkcijo uporabljam v ANTENA rezini 
{
    // to je potrebno se izboljsat, ne kaze najboljsih rezultatov!!! so saj konsistentni. a ce se nagjne dron, je veliko napake


    // tu je se delni izracun za pridobitev Z-kota (razporejam obremenitev rezin!)
    // v izracunu upostevamo popravek izracuna, ce je dron nagnjen -> preveri ce prav deluje
    // uporabljam korekcijske kote, ki so za eno periodo zakasnjeni -> v compassu je ze veliko napake, to vpliva neki
    float tmp_roll = kot_korek.XAxis* deg_to_rad;
    float tmp_pitch = kot_korek.YAxis* deg_to_rad;
    compass_komp.YAxis = compass_komp_tmp.XAxis * sin(tmp_roll)* sin(tmp_pitch) + compass_komp_tmp.YAxis * cos(tmp_roll) - compass_komp_tmp.ZAxis * sin(tmp_roll)*cos(tmp_pitch);
                                                                                
    // Calculate heading
    kot_merjeno.ZAxis = atan2(-compass_komp.YAxis, compass_komp.XAxis);

    // declination angle is 3'55E (positive)-> kot:+3.91667 -> kar je v radianih:0.068359 ==> DECLINATION
    // Formula: (deg + (min / 60.0)) / (180 / PI);
    kot_merjeno.ZAxis += DECLINATION;

    // Correct for heading < 0deg and heading > 360deg
    if (kot_merjeno.ZAxis < 0) {
        kot_merjeno.ZAxis += 2 * PI;
    }

    if (kot_merjeno.ZAxis > 2 * PI) {
        kot_merjeno.ZAxis -= 2 * PI;
    }

    // Convert to degrees
    kot_merjeno.ZAxis = kot_merjeno.ZAxis * rad_to_deg;
}
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
////////-----SCHEDULER--STANJE 3-KODA-------------------------------------------------------------------------------------------------
void get_stanje3_driver(void) {

    stanje = 3;    // preverjanje ce pride do prekoracitve casovne rezine da vemo kje je prislo do tezave
    Compass();     // traja max 92 od 110
    cas_S3 = TCNT0;
    if (cas_S3 > cas_S3_max) cas_S3_max = cas_S3;
    stanje = 0;
};
/*+++++++++++++ STANJE +++++++++++++*/

rtos_task_t task_STANJE3 = {
    get_stanje3_driver,     // function
    0,                      // last_tick
};