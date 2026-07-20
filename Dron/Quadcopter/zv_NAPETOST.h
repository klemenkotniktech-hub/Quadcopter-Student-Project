///////////// FUNKCIJA NAPETOSTI IN HITROSTI-- LINEARIZACIJA MODELA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//////////////////// Merjenje napetosti in izbira parametrov za dolocene razmere \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Glede na polnost baterije se doloci koeficiente za: (uporabimo histerezo, da ne bo med dvema napetostnima nivojema preklaplal)
//  - linearizacija regulacije (recimo stiri odseki za hitrost pri dani napetosti)

// Delitev po 6 napetostnih nivojih po 4 odsekih + delitev v odvisnosti od trenutne hitrosti!!!!!!!!!!
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// SELEKTIVNOST GLEDE NAPETOSTI \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 

void Napetost(void) {

    napetost = analogRead(A0) / 1023.0*5.0 * 3;  // napetost baterije  
                                                 // zaradi histereze gledamo se V_nivo:
    if (V_nivo == 1 && napetost >= 12.0) { Napetost_nivo1(); }
    else if (V_nivo == 1 && napetost < 12.0) {
        V_nivo = 2;
        Napetost_nivo2();
    }
    else if (V_nivo == 2 && napetost < 12.05 && napetost >= 11.4) { Napetost_nivo2(); }
    else if (V_nivo == 2 && napetost < 11.4) {
        V_nivo = 3;
        Napetost_nivo3();
    }
    else if (V_nivo == 3 && napetost < 11.45 && napetost >= 10.8) { Napetost_nivo3(); }
    else if (V_nivo == 3 && napetost < 10.8) {
        V_nivo = 4;
        Napetost_nivo4();
    }
    else if (V_nivo == 4 && napetost < 10.85 && napetost >= 10.2) { Napetost_nivo4(); }
    else if (V_nivo == 4 && napetost < 10.2) {
        V_nivo = 5;
        Napetost_nivo5();
    }
    else if (V_nivo == 5 && napetost < 10.25 && napetost >= 9.6) { Napetost_nivo5(); }
    else if (V_nivo == 5 && napetost < 9.6) {
        V_nivo = 6;
        Napetost_nivo6();
    }
    else { Napetost_nivo6(); }//POZOR, kmalu konec baterije!!!!
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// SELEKTIVNOST GLEDE HITROSTI \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 

void Napetost_nivo1(void)
{
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 11) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(11); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 12) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(12); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {/* primer 2 */
        if (S_nivo == 12) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(12); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 11) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(11); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 13) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(13); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 13) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(13); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 12) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(12); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 14) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(14); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 14) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(14); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 13) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(13); }                  // sicer vzamemo ze dolocene parametre
    };
};
void Napetost_nivo2(void)
{
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 21) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(21); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 22) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(22); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {	/* primer 2 */
        if (S_nivo == 22) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(22); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 21) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(21); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 23) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(23); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 23) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(23); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 22) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(22); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 24) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(24); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 24) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(24); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 23) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(23); }                  // sicer vzamemo ze dolocene parametre
    };
};
void Napetost_nivo3(void) {
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 31) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(31); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 32) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(32); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {	/* primer 2 */
        if (S_nivo == 32) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(32); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 31) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(31); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 33) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(33); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 33) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(33); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 32) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(32); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 34) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(34); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 34) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(34); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 33) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(33); }                  // sicer vzamemo ze dolocene parametre
    };
};
void Napetost_nivo4(void)
{
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 41) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(41); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 42) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(42); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {	/* primer 2 */
        if (S_nivo == 42) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(42); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 41) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(41); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 43) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(43); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 43) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(43); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 42) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(42); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 44) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(44); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 44) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(44); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 43) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(43); }                  // sicer vzamemo ze dolocene parametre
    };
};
void Napetost_nivo5(void) {
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 51) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(51); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 52) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(52); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {	/* primer 2 */
        if (S_nivo == 52) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(52); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 51) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(51); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 53) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(53); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 53) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(53); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 52) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(52); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 54) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(54); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 54) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(54); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 53) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(53); }                  // sicer vzamemo ze dolocene parametre
    };
};
void Napetost_nivo6(void)
{
    if (Glaven_pogon <= 600.0 && H_nivo == 1)
    {	/* primer 1 */
        if (S_nivo == 61) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(61); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 600.0 && H_nivo == 1)
    {
        H_nivo = 2;/*primer 2 */        // prehod: 1->2
        if (S_nivo == 62) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(62); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (580.0 < Glaven_pogon && Glaven_pogon <= 1100.0 && H_nivo == 2)
    {	/* primer 2 */
        if (S_nivo == 62) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(62); }                  // sicer vzamemo ze dolocene parametre  
    }
    else if (Glaven_pogon < 580.0 && H_nivo == 2)
    {
        H_nivo = 1;/* primer 1 */      // prehod 2->1
        if (S_nivo == 61) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(61); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (Glaven_pogon > 1100.0 && H_nivo == 2)
    {
        H_nivo = 3;/*primer 3 */    // prehod: 2->3
        if (S_nivo == 63) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(63); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1080.0 < Glaven_pogon && Glaven_pogon <= 1600.0 && H_nivo == 3)
    {	/* primer 3 */
        if (S_nivo == 63) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(63); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon < 1080 && H_nivo == 3)
    {
        H_nivo = 2;/* primer 2 */        // prehod: 3->2
        if (S_nivo == 62) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(62); }                  // sicer vzamemo ze dolocene parametre 
    }
    else if (Glaven_pogon > 1600 && H_nivo == 3)
    {
        H_nivo = 4;/* primer 4 */    // prehod: 3->4
        if (S_nivo == 64) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(64); }                  // sicer vzamemo ze dolocene parametre
    }
    else if (1580 <= Glaven_pogon && H_nivo == 4)
    {	/* primer 4 */
        if (S_nivo == 64) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(64); }                  // sicer vzamemo ze dolocene parametre 
    }
    else
    {
        H_nivo = 3; /* primer 3 */                // prehod: 4->3
        if (S_nivo == 63) { posodobitevRegulacije(0); }      // podatke posodobimo preko antene
        else { posodobitevRegulacije(63); }                  // sicer vzamemo ze dolocene parametre
    };
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// POSODOBITEV REGULACIJE-- IZBIRA KATERE KOEFICIENTE V DOLOCENEM SCENARIJU DOLOCI \\\\\\\\\\\\\\\\\\\\\\\\

void posodobitevRegulacije(int select)
{
    switch (select)
    {
    case 0:
        // Izberemo vrednosti z antene, sprotno umerjanje z daljinca
        ///// STABILIZACIJA \\\\\//
        // Regulator okoli X-osi
        korekcijaSZ_Xos = tmp_korekcijaSZ_Xos;
        roll_angle.P_gain = roll_angle.tmp_P_gain;
        roll_angle.I_gain = roll_angle.tmp_I_gain;
        roll_angle.D_gain = roll_angle.tmp_D_gain;
        // Regulator okoli Y-osi (pitch)
        //korekcijaSZ_Yos = tmp_korekcijaSZ_Yos;
        pitch_angle.P_gain = pitch_angle.tmp_P_gain;
        pitch_angle.I_gain = pitch_angle.tmp_I_gain;
        pitch_angle.D_gain = pitch_angle.tmp_D_gain;

        ///// RATE CONTROLER \\\\\//
        // Regulator okoli X-osi
        korekcijaSZ_Xos = tmp_korekcijaSZ_Xos;
        roll_rate.P_gain = roll_rate.tmp_P_gain;
        roll_rate.I_gain = roll_rate.tmp_I_gain;
        roll_rate.D_gain = roll_rate.tmp_D_gain;
        // Regulator okoli Y-osi (pitch)
        //korekcijaSZ_Yos = tmp_korekcijaSZ_Yos;
        pitch_rate.P_gain = pitch_rate.tmp_P_gain;
        pitch_rate.I_gain = pitch_rate.tmp_I_gain;
        pitch_rate.D_gain = pitch_rate.tmp_D_gain;
        break;

        /*	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////// NAPETOST = 1  (12.6V -:- 12V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */

    case 11: {/*NAPETOST = 1   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 12: { /*NAPETOST = 1   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo
               // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
               //korekcijaSZ_Xos = 0.8; */
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 13: {/*NAPETOST = 1   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 14: {/*NAPETOST = 1   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
             /* /////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////// NAPETOST = 2  (12.05V -:- 11.4V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */
    case 21: {/*NAPETOST = 2   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8; */
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 22: {/*NAPETOST = 2   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8; */
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 23: {/*NAPETOST = 2   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 24: {/*NAPETOST = 2   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
             /* /////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////// NAPETOST = 3  (11.45V -:- 10.8V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */
    case 31: {/*NAPETOST = 3   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 32: {/*NAPETOST = 3   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 33: {/*NAPETOST = 3   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 34: {/*NAPETOST = 4   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
             /* /////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////// NAPETOST = 4  (10.85V -:- 10.2V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */
    case 41: {/*NAPETOST = 4   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 42: {/*NAPETOST = 4   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 43: {/*NAPETOST = 4   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 44: {/*NAPETOST = 4   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
             /*  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////// NAPETOST = 5  (10.25V -:- 19.6V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ */
    case 51: {/*NAPETOST = 5   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 52: {/*NAPETOST = 5   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 53: {/*NAPETOST = 5   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 54: {/*NAPETOST = 5   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
             /* /////////////////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////// NAPETOST = 6  (9.65V -:- 9.0V)\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   */
    case 61: {/*NAPETOST = 6   HITROST = 1 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 62: {/*NAPETOST = 6   HITROST = 2 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 63: {/*NAPETOST = 6   HITROST = 3 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    case 64: {/*NAPETOST = 6   HITROST = 4 ->  Ze "uredu" parametri, le te sedaj preberemo, ker se tu nahajamo */
              // Regulator okoli X-osi  // MORAM SE NATANCNO DOLOCITI!!!
              //korekcijaSZ_Xos = 0.8;
        roll_angle.P_gain = 1;
        roll_angle.I_gain = 0;
        roll_angle.D_gain = 0;
        // Regulator okoli Y-osi   // MORAM SE NATANCNO DOLOCITI!!!
        //korekcijaSZ_Yos = 0.8;
        pitch_angle.P_gain = 1;
        pitch_angle.I_gain = 0;
        pitch_angle.D_gain = 0;
        break;
    }
    default: {  // Izberemo vrednosti z antene, sprotno umerjanje z daljinca
                // Regulator okoli X-osi
                //korekcijaSZ_Xos = tmp_korekcijaSZ_Xos;
        roll_angle.P_gain = roll_angle.tmp_P_gain;
        roll_angle.I_gain = roll_angle.tmp_I_gain;
        roll_angle.D_gain = roll_angle.tmp_D_gain;
        // Regulator okoli Y-osi
        //korekcijaSZ_Yos = tmp_korekcijaSZ_Yos;
        pitch_angle.P_gain = pitch_angle.tmp_P_gain;
        pitch_angle.I_gain = pitch_angle.tmp_I_gain;
        pitch_angle.D_gain = pitch_angle.tmp_D_gain;
        break;
    }

    }
}


///-------------okoli z-osi (Yaw)------------------------/////////////////////////////////////////////////////////////////////////////////////////
//imamo tezava, da se kot meri od: 0-->360, tako da moramo sedaj paziti prehod med 0 in 360
/*
!!!!!!!!  KO BOS RABIL TO, IMPLEMENTIRAJ. DO TAKRAT NE IZGUBLJAJ TU CAS!!!!!

if ((180 < abs(izmerjen_kot_z - zeljen_kot_z)))
{ //smo okoli 180, tezava, ce se izmerjeni in zeljena vrednost nahajata ena na 179 in druga -179
if ((izmerjen_kot_z - zeljen_kot_z) < 0)
{   //sedaj pristejemo 360
pid_error_temp = izmerjen_kot_z - zeljen_kot_z + 360;
}
else
{                                        //tu moramo odsteti 360
pid_error_temp = izmerjen_kot_z - zeljen_kot_z - 360;
}
}
else
{   //lahko normalno poracunamo
pid_error_temp = izmerjen_kot_z - zeljen_kot_z;
};

/// NOVO--> ko bo  |pid_output_roll| == pid_max_roll --> takrat izklopimo lezenje integralnega clena,sicer gre v nasicenje
if (abs(abs(pid_output_yaw) - pid_max_yaw) > 5)
{
pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
}; //izracun Integralske napake(zajema vse prejsne vzorce)


if (pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
else if (pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
if (pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
else if (pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

pid_last_yaw_d_error = pid_error_temp;
*/