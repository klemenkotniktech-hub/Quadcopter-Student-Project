///////////////////Trenutne razmere koeficientov //////////////////
void displ(void) {
    Serial.println(""); Serial.println(""); Serial.println(""); Serial.println("");
    Serial.println("Trenutne maksimalne razmere koeficientov:");
    Serial.print("korekcijaSZ_Yos_maks:  ");
    Serial.print(korekcijaSZ_Yos_maks);

    Serial.print("korekcijaSZ_Xos_maks:  ");
    Serial.print(korekcijaSZ_Xos_maks);

    Serial.print("  pid_p_gain_roll_maks: ");
    Serial.print(pid_p_gain_maks);

    Serial.print("  pid_i_gain_roll_maks: ");
    Serial.print(pid_i_gain_maks);

    Serial.print("  pid_d_gain_roll_maks: ");
    Serial.print(pid_d_gain_maks);



    Serial.println(""); Serial.println(""); Serial.println("");

    ////////Sklic naslednih ukazov\\\\\\\\\\\\\\\\\\\\\\\\\\\
            Serial.print("Popravek koeficientov:  ");
    Serial.print("korekcijaSZ_Yos_maks-- 1   ");
    Serial.print("pid_p_gain_roll_maks-- 2  ");
    Serial.print("pid_i_gain_roll_maks-- 3  ");
    Serial.print("pid_d_gain_roll_maks-- 4  ");
    Serial.print("korekcijaSZ_Xos_maks-- 5  ");
    Serial.println("konec-- 0 ");
}

///////////////////Branje vrednosti preko Serial.read\\\\\\\\\\\\\\\\\\\\\\\\

double branje(double j) {
    int stev[10] = { 0,0,0,0,0,0,0,0,0,0 };
    int i = 0;
    int tmp = 0;
    double del = 1;
    int del_tmp = 0;
    int del_konec_tmp = 0;
    double predznak = 1;
    double stevilo = 0;

    while (Serial.available() > 0) {//dokler so informacije na voljo, toliko casa beremo--> eno crko na enkrat
        delay(20);
        tmp = Serial.read();            // 0 = 48, 9 = 57,    ,=44 in -=45
        if (tmp > 47) {            //stevilo 0-:-9
            stev[i] = tmp - 48;
            i++;
        }
        else if (tmp == 44 || tmp == 46) {
            if (i <1)break;         //ce je vejica prvi znak, prekinemo ukaz branja, vrnemo 0
            del_tmp = i;           //decimalni index vejice
        }
        else { predznak = -1; };   //ce imamo negativno vrednost
    };////////sedaj imamo vsa zapisana stevila, decimalno vejico in predznak
    del_konec_tmp = i;      //shranimo podatek koliko je bilo vseh podatkov+1--> uporabimo pri postavitvi decimalne vejice
                            ///////sedaj zdruzimo dobljene poodatke--> to le stevilka, brez decimalne vejie\\\\\\\\\\\\\\\\
    
    for(i = 0; i < del_konec_tmp; i++ )
    {
        stevilo = stevilo * 10 + stev[i];
    };
/*    Tega ne upostevamo, ker bomo dejanske vrednosti dobili preko potenciometra, tu le dolocimo maks vrednost
if(j == 1){    korekPr_SZ_vred = stevilo;
}else if(j == 2){ pid_p_gain_vred = stevilo;
}else if (j == 3){pid_i_gain_vred = stevilo;
}else if(j == 4){pid_d_gain_vred = stevilo;
}if(j == 5){  pid_max_vred = stevilo;};
*/
/////////preverimo decimalno vejico
    if (del_tmp >0) 
    {
        for (i = 0; i <(del_konec_tmp - del_tmp); i++) { del = del * 10; };
        stevilo = stevilo / del;
    };
/*
if(j == 1){  korekPr_SZ_D = del;
}else if(j == 2){ pid_p_gain_D = del;
}else if (j == 3){pid_i_gain_D = del;
}else if(j == 4){pid_d_gain_D = del;
}if(j == 5) { pid_max_D = del;}
*/
    if (predznak < 0) { stevilo = stevilo*(-1); }
////////////////////////// Zapis koncne maksimalne vrednosti vrednosti--> kamor bodo segli potenciometri\\\\\\\\\\\\\\\\\\\\\\\\

/*  ta del se uposteva ko klicemo posamicno funkcijo in ji dolocimo vrednosti, nasledna spodnja funkcija
    if (j == 1) {
    korekPr_SZ_maks = del;
}
else if (j == 2) {
    pid_p_gain_maks = del;
}
else if (j == 3) {
    pid_i_gain_maks = del;
}
else if (j == 4) {
    pid_d_gain_maks = del;
}if (j == 5) { pid_max_vred = del; }
*/

return stevilo;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////Funkcije korekcijskih faktorjev\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void korekcijaYos(void)
{
    Serial.println(""); Serial.println(""); Serial.println("");
    Serial.print("Trenutna maksimalna vrednost korekcijskega faktorja sprednjega pogona:  ");
    Serial.println(korekcijaSZ_Yos_maks);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_korekPr = 1;
    while (out_korekPr > 0) {
        if (Serial.available() > 0) {
            delay(100);
            korekcijaSZ_Yos_maks = branje(data);  // data je enak 1
            Serial.print("vrednost:  ");
            Serial.print(korekcijaSZ_Yos_vred);
            Serial.print("Decimalna vejica:  ");
            Serial.println(korekcijaSZ_Yos_D);
            Serial.print("Koncna maks vrednost korekcijskega faktorja SZ_Yos:  ");
            Serial.print(korekcijaSZ_Yos_maks);
            Serial.println(""); Serial.println(""); Serial.println("");
            if (korekcijaSZ_Yos < 0) {
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
            }
            else { out_korekPr = 0; }
        };
    };
};
void pid_p(void) 
{
    Serial.println(""); Serial.println(""); Serial.println("");
    Serial.print("Trenutna maks vrednost pid_p_gain:  ");
    Serial.println(pid_p_gain_maks);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_p = 1;
    while (out_pid_p > 0) {
        if (Serial.available() > 0) {
            delay(100);
            pid_p_gain_maks = branje(data);  // data je enak 2
            Serial.print("vrednost:  ");
            Serial.print(pid_p_gain_vred);
            Serial.print("Decimalna vejica:  ");
            Serial.println(pid_p_gain_D);
            Serial.print("Maks vrednost P-gain:  ");
            Serial.print(pid_p_gain_maks);
            Serial.println(""); Serial.println(""); Serial.println("");
            if (pid_p_gain_roll < 0) {
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
            }
            else { out_pid_p = 0; }
        };
    };
};
void pid_i(void) 
{
    Serial.println(""); Serial.println(""); Serial.println("");
    Serial.print("Trenutna maks vrednost pid_i_gain:  ");
    Serial.println(pid_i_gain_maks);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_i = 1;
    while (out_pid_i > 0) {
        if (Serial.available() > 0) {
            delay(100);
            pid_i_gain_maks = branje(data);    // data je tu 3
            Serial.print("vrednost:  ");
            Serial.print(pid_i_gain_vred);
            Serial.print("Decimalna vejica:  ");
            Serial.println(pid_i_gain_D);
            Serial.print("Maks vrednost I-gain:  ");
            Serial.print(pid_i_gain_maks);
            Serial.println(""); Serial.println(""); Serial.println("");
            if (pid_i_gain_roll < 0) {
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
            }
            else { out_pid_i = 0; }
        };
    };
};
void pid_d(void) 
{
    Serial.println(""); Serial.println(""); Serial.println("");
    Serial.print("Trenutna maks vrednost pid_d_gain:  ");
    Serial.println(pid_d_gain_maks);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_d = 1;
    while (out_pid_d > 0) {
        if (Serial.available() > 0) {
            delay(100);
            pid_d_gain_maks = branje(data);  // data je 4
            Serial.print("vrednost:  ");
            Serial.print(pid_d_gain_vred);
            Serial.print("Decimalna vejica:  ");
            Serial.println(pid_d_gain_D);
            Serial.print("Maks vrednost D-gain:  ");
            Serial.print(pid_d_gain_maks);
            Serial.println(""); Serial.println(""); Serial.println("");
            if (pid_d_gain_roll < 0) {
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
            }
            else { out_pid_d = 0; }
        };
    };
};
void korekcijaXos(void) 
{
    Serial.println(""); Serial.println(""); Serial.println("");
    Serial.print("Trenutna maksimalna vrednost korekcijskega faktorja zadnjega pogona(X-os):  ");
    Serial.println(korekcijaSZ_Xos_maks);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_korekPr = 1;
    while (out_korekPr > 0) {
        if (Serial.available() > 0) {
            delay(100);
            korekcijaSZ_Xos_maks = branje(data);  // data je enak 1
            Serial.print("vrednost:  ");
            Serial.print(korekcijaSZ_Xos_vred);
            Serial.print("Decimalna vejica:  ");
            Serial.println(korekcijaSZ_Xos_D);
            Serial.print("Koncna maks vrednost korekcijskega faktorja SZ_Xos:  ");
            Serial.print(korekcijaSZ_Xos_maks);
            Serial.println(""); Serial.println(""); Serial.println("");
            if (korekcijaSZ_Xos < 0) {
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
            }
            else { out_korekPr = 0; }
        };
    };
};

//// glavna funkcija za spreminjanje obmocja spremenljivk
void Koef_spreminjanje(void) {
    if (Serial.available() > 0) {    //pogleda ce smo prejeli kaksno informacijo preko PC-ja
        delay(100);
        int loop_counter = 0;
        while (Serial.available() > 0) { delay(15); loop_counter = Serial.read(); }        //Empty the Serial buffer.
        loop_counter = 0;
        data = 6;      //da vstopimo v naslednjo while-zanko


        while (data > 0.02) {
            displ();  //predstavi trenutne vrednosti in ponudi moznosti za naprej
            int out = 1;
            while (out > 0) {
                if (Serial.available() > 0) {
                    out = 0;
                    data = branje(0);
                    loop_counter = 0;

                    ////////////////////////Sedaj smo prebrali zeljeno opcijo, ter po tej opciji se bomo vrnili nazaj na seznam zelja
                    Serial.print("Data je:   ");
                    Serial.println(data, 5);
                    if (data > 0.98 && data < 1.02)korekcijaYos();
                    if (data > 1.98 && data < 2.02)pid_p();
                    if (data > 2.98 && data < 3.02)pid_i();
                    if (data > 3.98 && data < 4.02)pid_d();
                    if (data > 4.98 && data < 5.02)korekcijaXos();

                    while (Serial.available() > 0) { delay(15); loop_counter = Serial.read(); }        //Empty the Serial buffer.              
                }
            }
        }/////konec while
        while (Serial.available() > 0) { delay(15); loop_counter = Serial.read(); }        //Empty the Serial buffer.
    }
};

