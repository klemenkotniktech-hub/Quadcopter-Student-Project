// Funkcije za podporo motorjev
#define MOT_MAKS 1850
#define MOT_MIN 0
///////////////////////MOTOR FUNKCIJE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
////////////////////////////////////////////////////////////////////////////////////////////////
//------inicializacija funkcije v setup-u:
void init_motor(void) {
    Mot_SD.attach(10);  //desno spredaj-oranzen
    Mot_SD.writeMicroseconds(1000);  //najmanjsa vrednost ki jo moras poslati na ESC, da so motorji v pripravljenosti

    Mot_ZD.attach(9);      //desno zadaj, crn
    Mot_ZD.writeMicroseconds(1000);

    Mot_ZL.attach(6);      //levo zadaj-crn
    Mot_ZL.writeMicroseconds(1000);

    Mot_SL.attach(5);      //levo spredaj-oranzen
    Mot_SL.writeMicroseconds(1000);
}
/////////////////////////////////////////////////////////////////////////////

void motorji_driver(void)
{
    stanje = 6;
    if (motor_counter > 750 && Izklop_pogona == 0)
    { // 1 sekunda ~= 100 ciklov;  6sekund dron ne bo odziven, da ne bi kdo ponesreci spremenil nastavitev ESC-ja

        if (Glaven_pogon > Prag_izklop_reg)
        {   //   dejansko uporaba: 0-:-2000, da se motorji ne obremenijo nad 85%
            //////////////////POPRAVEK POGON PID \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

            stanje = 7;  //reference, ce preseze dolzino casovno rezine, da vem do kam je prisel program
            // tu podajam OUTPUT RATE controlerja, zato so tu vrednosti 'rate', output stabilizacije se MORA preimenovati v zeleni RATE            
            val_Mot_SD = Glaven_pogon - korekcijaSZ_Xos * roll_rate.PID_output - pitch_rate.PID_output;// +yaw.PID_output; //Calculate the pulse for esc 1 (front-right - CCW) Beli
            val_Mot_ZD = Glaven_pogon - korekcijaSZ_Xos * roll_rate.PID_output + pitch_rate.PID_output;// -yaw.PID_output; //Calculate the pulse for esc 2 (rear -right - CW)  Crni
            val_Mot_ZL = Glaven_pogon + roll_rate.PID_output                   + pitch_rate.PID_output;// +yaw.PID_output; //Calculate the pulse for esc 3 (rear -left - CCW)  Crni
            val_Mot_SL = Glaven_pogon + roll_rate.PID_output                   - pitch_rate.PID_output;// -yaw.PID_output; //Calculate the pulse for esc 4 (front-left - CW)  Beli
                                                                                     // pozor, te korekcijske faktorje lahko se razdelim na obmocja delovanja, tako da ne bo na celotnem obmocju deloval le en priblizek
                                                                                     // !!! moras se implementirati regulacijo okoli Z-osi, takrat tudi poglej predznak za vsak motor!!!!!!

                                                                                    
            //preureditev vrednosti za zapis v ESC, ki vrednost razpne med: 1000---2000, da motorji niso preobremenjeni in se ne bi poskodovali
            //preverimo, da vrednosti esc-ja niso presegle 2000, (to je izvedljivo, imamo del neizkoriscenega potenciala, a zato boljso odzivnost)
            val_Mot_SD = limit_val_Mot(val_Mot_SD);
            val_Mot_ZD = limit_val_Mot(val_Mot_ZD);
            val_Mot_ZL = limit_val_Mot(val_Mot_ZL);
            val_Mot_SL = limit_val_Mot(val_Mot_SL);
            
            //imamo se varnostno mejo, da motorje ne preobremenimo, zato gredo lahko te max do 87%
            pretvorbaVrednosti(val_Mot_SD, val_Mot_ZD, val_Mot_ZL, val_Mot_SL);  //pretvorb vrednosti na pravo obmocje za posiljanje podatkov na ESC
            stanje = 8;
        }
        else
        {   //ce je opogon manjsi od Prag_izklop_reg-> takrat dron se ne more vzleteti, in tu ne smemo prekmalu upostevati PID-ja
            //->lahko se nabere prevelika napaka
            pretvorbaVrednosti(Glaven_pogon, Glaven_pogon, Glaven_pogon, Glaven_pogon);
        };
    }
    else
    {   //prvih 5 sekund, na vhod ESC-ja podamo minimalno vrednost, tako da ne spremenimo nastavitev
        pretvorbaVrednosti(0, 0, 0, 0);
        motor_counter++;      //povecamo counter, ta se bo poveceval le nazacetku, dokler bomo padli v to zanko
    };
    //// zapis vrednosti na vhod ESC-ja

    Mot_SD.writeMicroseconds(val_Mot_SD);
    Mot_ZD.writeMicroseconds(val_Mot_ZD);
    Mot_ZL.writeMicroseconds(val_Mot_ZL);
    Mot_SL.writeMicroseconds(val_Mot_SL);
};
/////////////////////////////////////////////////////////////////////////////

void pretvorbaVrednosti(double val1, double val2, double val3, double val4)
{   //pretvorb vrednosti na pravo obmocje za posiljanje podatkov na ESC
    val_Mot_SD = map(val1, 0, 2000, 1000, 2000); //moram popraviti PID koeficiente, da bodo bolj sorazmerni s pogonom
    val_Mot_ZD = map(val2, 0, 2000, 1000, 2000); //moram popraviti PID koeficiente, da bodo bolj sorazmerni s pogonom
    val_Mot_ZL = map(val3, 0, 2000, 1000, 2000); //moram popraviti PID koeficiente, da bodo bolj sorazmerni s pogonom
    val_Mot_SL = map(val4, 0, 2000, 1000, 2000); //moram popraviti PID koeficiente, da bodo bolj sorazmerni s pogonom
};
/////////////////////////////////////////////////////////////////////////////

float limit_val_Mot(float val_mot)
{
    // ce so vrednosti previsoke
    if (val_mot > MOT_MAKS)
    {
        val_mot = MOT_MAKS;
    };
    //ce so vrednosti prenizke
    if (val_mot < MOT_MIN)
    {
        val_mot = MOT_MIN;
    };
    return val_mot;
};
/////////////////////////////////////////////////////////////////////////////