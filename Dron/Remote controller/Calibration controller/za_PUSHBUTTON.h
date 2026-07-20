//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// DODANE FUNKCIJE PUSH BUTTON \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

int PUSHBUTTON(void) {    // koda okrnjena le za S1, S2 in S3
    int S = 0;// zacasna spremenljivka
              /*
              // Prva vrstica
              digitalWrite(vr1,LOW);
              tmp[0] = digitalRead(st1);
              if(tmp[0] ==0 && tmp[1]==1) {tmp[1]=0; S=1; Serial.println("  Gump S1");}; if(tmp[0]==1){tmp[1]=1;};
              tmp[0] = digitalRead(st2);
              if(tmp[0] == 0 && tmp[2]==1) {tmp[2]=0;S=2; Serial.println("  Gump S2");}if(tmp[0]==1){tmp[2]=1;};
              tmp[0] = digitalRead(st3);
              if(tmp[0] == 0 && tmp[3] ==1) {tmp[3]=0; S=3; Serial.println("  Gump S3");}if(tmp[0]==1){tmp[3]=1;};
              tmp[0] = digitalRead(st4);
              if(tmp[0] == 0 && tmp[4] == 1) {tmp[4]=0; S=4; Serial.println("  Gump S4");}if(tmp[0]==1){tmp[4]=1;};

              digitalWrite(vr1,HIGH);

              // Druga vrstica
              digitalWrite(vr2,LOW);
              tmp[0] = digitalRead(st1);
              if(tmp[0] == 0 && tmp[5]==1) {tmp[5] =0; S=5; Serial.println("  Gump S5");}if(tmp[0]==1){tmp[5]=1;};
              tmp[0] = digitalRead(st2);
              if(tmp[0] == 0 && tmp[6]==1) {tmp[6]=0; S=6; Serial.println("  Gump S6");}if(tmp[0]==1){tmp[6]=1;};
              tmp[0] = digitalRead(st3);
              if(tmp[0] == 0 && tmp[7]==1) {tmp[7]=0; S=7; Serial.println("  Gump S7");}if(tmp[0]==1){tmp[7]=1;};
              tmp[0] = digitalRead(st4);
              if(tmp[0] == 0 && tmp[8]==1) {tmp[8]=0; S=8; Serial.println("  Gump S8");}if(tmp[0]==1){tmp[8]=1;};
              digitalWrite(vr2,HIGH);
              // Tretja vrstica
              digitalWrite(vr3,LOW);
              tmp[0] = digitalRead(st1);
              if(tmp[0] == 0 && tmp[9]==1) {tmp[9]=0; S=9; Serial.println("  Gump S9");}if(tmp[0]==1){tmp[9]=1;};
              tmp[0] = digitalRead(st2);
              if(tmp[0] == 0 && tmp[10]==1) {tmp[10]=0; S=10; Serial.println("  Gump S10");}if(tmp[0]==1){tmp[10]=1;};
              tmp[0] = digitalRead(st3);
              if(tmp[0] == 0 && tmp[11]==1) {tmp[11]=0; S=11; Serial.println("  Gump S11");}if(tmp[0]==1){tmp[11]=1;};
              tmp[0] = digitalRead(st4);
              if(tmp[0] == 0 && tmp[12]==1) {tmp[12]=0; S=12; Serial.println("  Gump S12");}if(tmp[0]==1){tmp[12]=1;};
              digitalWrite(vr3,HIGH);
              // Cetrta vrstica
              digitalWrite(vr4,LOW);
              tmp[0] = digitalRead(st1);
              if(tmp[0] == 0 && tmp[13]==1) {tmp[13]=0; S=13; Serial.println("  Gump S13");}if(tmp[0]==1){tmp[13]=1;};
              tmp[0] = digitalRead(st2);
              if(tmp[0] == 0 && tmp[14]==1) {tmp[14]=0; S=14; Serial.println("  Gump S14");}if(tmp[0]==1){tmp[14]=1;};
              tmp[0] = digitalRead(st3);
              if(tmp[0] == 0 && tmp[15]==1) {tmp[15]=0; S=15; Serial.println("  Gump S15");}if(tmp[0]==1){tmp[15]=1;};
              tmp[0] = digitalRead(st4);
              if(tmp[0] == 0 && tmp[16]==1) {tmp[16]=0; S=16; Serial.println("  Gump S16");}if(tmp[0]==1){tmp[16]=1;};
              digitalWrite(vr4,HIGH);
              */
    return S;
};

