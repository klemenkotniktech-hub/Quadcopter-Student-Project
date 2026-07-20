void potencRead(void) {
    // Analog0 : Merimo throttle speed
    pogon = (analogRead(A0))*2000.0 / 1023.0;

    if (pogon > 1950) { pogon = 2000; }     // za umerjanje
    if (pogon < 50) { pogon = 0; }           // za umerjanje
    pogon_vred = pogon* pogon_D;  // to vrednost in decimalni zamik posljemo preko antene, nato tam naredimo pretvorbo nazaj

    // Analog2 : Merimo Korekcijski faktor Spredaj-Zadaj_Xos
    korekcijaSZ_Xos = (analogRead(A2)) / 1023.0* korekcijaSZ_Xos_maks;
    korekcijaSZ_Xos_vred = korekcijaSZ_Xos * korekcijaSZ_Xos_D;

    // Analog3 : Merimo PID P-gain:
    pid_p_gain_roll = analogRead(A3) / 1023.0*pid_p_gain_maks;
    pid_p_gain_vred = pid_p_gain_roll* pid_p_gain_D;

    // Analog4 : Merimo PID I_gain:
    pid_i_gain_roll = analogRead(A4) / 1023.0*pid_i_gain_maks;
    pid_i_gain_vred = pid_i_gain_roll* pid_i_gain_D;

    // Analog5 : Merimo PID D_gain:
    pid_d_gain_roll = analogRead(A5) / 1023.0*pid_d_gain_maks;
    pid_d_gain_vred = pid_d_gain_roll* pid_d_gain_D;


    /*
    // Analog5 : Merimo PID maks vrednost:
    pid_max_roll = analogRead(A2)/1023.0*pid_max_maks;
    pid_max_vred = pid_max_roll* pid_max_D;
    */
};