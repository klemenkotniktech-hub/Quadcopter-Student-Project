//koda daljinca: izvedba meritev, nato oddaja podatkov, ni casovnega rezinjenja
//               normalno deluje(brez prekinitev), ker je tako 



#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";
//----------- Digitalni pini
int pin_levo = 3;
int pin_desno = 2;
//------------ inicializacija spremenljivk
double pogon = 0;
double kot_x_os = 0;
double kot_y_os = 0;
int kot = 0;
int jakost_odziva = 0;
int levo,desno = 0;

double data;    //prebrani podatek preko PC-ja
double korekPr_SZ = 0.8;
double korekPr_D = 10; 
double korekPr_vred = 8;

double pid_p_gain_roll = 0;
double pid_p_gain_D = 1;
double pid_p_gain_vred = 0;

double pid_i_gain_roll = 0 ;
double pid_i_gain_D = 1;
double pid_i_gain_vred = 0;

double pid_d_gain_roll = 0;
double pid_d_gain_D = 1;
double pid_d_gain_vred = 0;

double pid_max_roll = 400; 
double pid_max_D = 1;
double pid_max_vred = 400;


///spremenljivke za funkcijo potencM
double potencVred = 0;
double DekadRazred = 1;
double range = 400;
int izbor = 1;
/////////////Nastavljanje korekcijske faktorje \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
int out = 1;
int loop_counter = 0;  //za spraznenje Serial buffer
void PID_koef(void);
void displ(void);  //predstavi trenutne vrednosti in ponudi moznosti za naprej
void korekPr(void);
void pid_p(void);
void pid_i(void);
void pid_d(void);
void pid_max(void);
double branje(double j);     // branje Serial-PC
void potencM(void);          //branje tenciometra za fino nastavljanje zeljene vrednosti
void kalibPotenc(void);      //preko SerialLink PC lahko izberemo to opcijo in rocno nastavimo obmocje ter za katero velicino nastavljamo vrednost 
void IZBOR();                // pomozna funkcija za dolocitev katero vrednost bomo opisali s potenciometrom
void RANGE();                // pomozna funkcija za dolocanja obmocja pri nastavljanju obmocja potenciometra  
void DEKADA();               // pomozna funkcija za dolocanja dekade pri nastavljanju obmocja potenciometra 
//--------------------------Setup------------
void setup() {/////////////////////////////////////////
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
  Serial.begin(9600);
  
  //inicializacija vhodnih Digitalnih pinov:
  pinMode(pin_levo,INPUT);
  pinMode(pin_desno,INPUT);
  
}///////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() { // da pride naokoli funkcija(ce sprejemnik ne sprejme->veckrat poskusi oddat)= 72 200microsekund
  
  //branje vrednosti pogona:  (offset = 59)
  pogon = (analogRead(A2)-62)*1800.0/862;
  //----branje kota ter jakost odziva:---------------
  kot_y_os =  ((analogRead(A0)-513)*45.0/513);
  kot_x_os =  ((analogRead(A1)-505)*(-45.0)/518);
  /*
  Serial.println("kot_x.os");
  Serial.println(kot_x_os);
  Serial.println("kot_y.os");
  Serial.println(kot_y_os);
  Serial.println("pogon:");
  */
  //Serial.println(pogon*1000/1800.0);  //podatek koliko msekund gre na ESC
  
  //  branje zavoja LEVO  ter DESNO
  levo = digitalRead(pin_levo);  // 1- roteraj
  desno = digitalRead(pin_desno);// 0- miruj
  
  if (desno & levo ){ //prepovedano stanje!!!
   desno = 0;
   levo = 0; 
  }
  //////// PREKO POTENCIOMETRA REALNO CASNO SPREMINJAMO VREDNOST\\\\\\\\\\\\\\\\\
  potencM();
  Serial.print("Izbor:  ");
  Serial.print(izbor);
  Serial.print("Vrednost na potenciometru:  ");
  Serial.println(potencVred/DekadRazred);
  
  ///////////////////spreminjanje koeficientov PID- regulatorja\\\\\\\\\\\\\\\\\\\\\\\qq
  PID_koef();
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //--------------- posiljanje podatkov--------------------
  
//podatkovni vrstni red: moc pogona, kot_x_os,kot_y_os, zavoj levo(0-NE,1-JA),zavoj desno;-->sledijo podatki o parametrih PID-regulatorja
//podatki: korekPr_vrednost,korekPr_DecimalnoM || pid_p_gain_vred, pid_p_gain_D || pid_i_gain_vred, pid_i_gain_D|| pid_d_gain_vred,pid_d_gain_D||pid_max_vred,pid_max_D
// Dodati moram se nastavljanje okoli Z-osi
      const int paket[] ={pogon,kot_x_os,kot_y_os,levo,desno,  korekPr_vred,korekPr_D,  pid_p_gain_vred,pid_p_gain_D,  pid_i_gain_vred,pid_i_gain_D,  pid_d_gain_vred,pid_d_gain_D,  pid_max_vred,pid_max_D }; 
  radio.write(&paket, sizeof(paket));  //paket bo ponavljajoce oddajal 71 500microsekund
  
}//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////STRANSKE FUNKCIJE\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
////////////UMERJANJE KOEFICIENTOV PID\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
void PID_koef(void){
  if(Serial.available() > 0){    //pogleda ce smo prejeli kaksno informacijo preko PC-ja
    delay(100);
    while(Serial.available() > 0){delay(15);loop_counter = Serial.read();}        //Empty the Serial buffer.
    loop_counter = 0;
    data = 6;      //da vstopimo v naslednjo while-zanko
    
    
    while(data > 0.02){
        displ();  //predstavi trenutne vrednosti in ponudi moznosti za naprej
        out = 1;
        while(out > 0){
            if(Serial.available() > 0){
              out = 0;
              data = branje(0);
              loop_counter = 0;
              
              ////////////////////////Sedaj smo prebrali zeljeno opcijo, ter po tej opciji se bomo vrnili nazaj na seznam zelja
              Serial.print("Data je:   ");
              Serial.println(data,5);
              if(data > 0.98 && data < 1.02)korekPr();
              if(data > 1.98 && data < 2.02)pid_p();
              if(data > 2.98 && data < 3.02)pid_i();
              if(data > 3.98 && data < 4.02)pid_d();
              if(data > 4.98 && data < 5.02)pid_max();
              if(data > 5.98 && data < 6.02)kalibPotenc();
               while(Serial.available() > 0){delay(15);loop_counter = Serial.read();}        //Empty the Serial buffer.              
              }
              }
          }/////konec while
          while(Serial.available() > 0){delay(15);loop_counter = Serial.read();}        //Empty the Serial buffer.
    }
};



///////////////Funkcije korekcijskih faktorjev///////////////////////////
void korekPr(void){
  Serial.print("Trenutni korekcijski faktor sprednjega pogona:  ");
  Serial.println(korekPr_SZ);
  Serial.println("Vpisi novo zeljeno vrednost:");
  int out_korekPr = 1;
  while( out_korekPr > 0){
      if(Serial.available() > 0){
          delay(100);
          korekPr_SZ = branje(data);
          Serial.print("vrednost:  ");
          Serial.print(korekPr_vred);
          Serial.print("Decimalna vejica:  ");
          Serial.println(korekPr_D);
          if(korekPr_SZ < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_korekPr = 0;}
       };
  };
};
void pid_p(void){
    Serial.print("Trenutni pid_p_gain:  ");
    Serial.println(pid_p_gain_roll);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_p = 1;
    while( out_pid_p > 0){
      if(Serial.available() > 0){
          delay(100);
          pid_p_gain_roll = branje(data);
          Serial.print("vrednost:  ");
          Serial.print(pid_p_gain_vred);
          Serial.print("Decimalna vejica:  ");
          Serial.println(pid_p_gain_D);
          if(pid_p_gain_roll < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_pid_p = 0;}
       };
  };
};
void pid_i(void){
    Serial.print("Trenutni pid_i_gain:  ");
    Serial.println(pid_i_gain_roll);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_i = 1;
    while( out_pid_i > 0){
      if(Serial.available() > 0){
          delay(100);
          pid_i_gain_roll = branje(data);
          Serial.print("vrednost:  ");
          Serial.print(pid_i_gain_vred);
          Serial.print("Decimalna vejica:  ");
          Serial.println(pid_i_gain_D);
          if(pid_i_gain_roll < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_pid_i = 0;}
       };
  };
};
void pid_d(void){
    Serial.print("Trenutni pid_d_gain:  ");
    Serial.println(pid_d_gain_roll);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_d = 1;
    while( out_pid_d > 0){
      if(Serial.available() > 0){
          delay(100);
          pid_d_gain_roll = branje(data);
          Serial.print("vrednost:  ");
          Serial.print(pid_d_gain_vred);
          Serial.print("Decimalna vejica:  ");
          Serial.println(pid_d_gain_D);
          if(pid_d_gain_roll < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_pid_d = 0;}
       };
  };
};
void pid_max(void){
    Serial.print("Trenutni pid_max:  ");
    Serial.println(pid_max_roll);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_pid_max = 1;
    while( out_pid_max > 0){
      if(Serial.available() > 0){
          delay(100);
          pid_max_roll = branje(data);
          Serial.print("vrednost:  ");
          Serial.print(pid_max_vred);
          Serial.print("Decimalna vejica:  ");
          Serial.println(pid_max_D);
          if(pid_max_roll < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_pid_max = 0;}
       };
  };
  
};
void kalibPotenc(void){
  int out_kalibPotenc = 1;
  while(out_kalibPotenc > 0){
        Serial.print("izbor: ");
        Serial.print(izbor);
        Serial.print(" potencVred: ");
        Serial.print(potencVred);
        Serial.print("  range: ");
        Serial.print(range);
        Serial.print("  DekadRazred: ");
        Serial.println(DekadRazred);
        Serial.println("Izberi:  izbor: '1'    range: '2'   DekadRazred: '3'    izhod: '0'");
        
        double tmpKalib = 6;
        int out_K = 1;
        while( out_K > 0){
          if(Serial.available() > 0){
              delay(100);
              out_K = 0;
              tmpKalib = branje(data);
              
              //tu moras na podlagi razlicne vrednosti --> v tiste zapisat nove spremenljivke
              if(tmpKalib > 0.98 && tmpKalib < 1.02)IZBOR();  //jih moras se definirat
              if(tmpKalib > 1.98 && tmpKalib < 2.02)RANGE();  //jih moras se definirat
              if(tmpKalib > 2.98 && tmpKalib < 3.02)DEKADA();  //jih moras se definirat
              if(tmpKalib == 0)out_kalibPotenc = 0;
              while(Serial.available() > 0){delay(15);loop_counter = Serial.read();}        //Empty the Serial buffer. 
              if(tmpKalib < 0){
                Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
              }
           };
    };
  };
};
void IZBOR(void){
  Serial.print("Trenutna vrednost izbor-a:  ");
    Serial.println(izbor);
    Serial.println("Vpisi novo zeljeno vrednost:");
    Serial.println("korekPr_D:'1' pid_p_gain:'2' pid_i_gain:'3'  pid_d_gain:'4'  pid_max:'5' "); // prikaz kaj moramo pritisniti za doloceno vrednost
    int out_IZBOR = 1;
    
    while( out_IZBOR > 0){
      if(Serial.available() > 0){
          delay(100);
          izbor = branje(data);
          Serial.print("vrednost izbora:  ");
          Serial.print(izbor);
          if(izbor < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_IZBOR = 0;};
       };
  };
};
void RANGE(void){
    Serial.print("Trenutna vrednost range-a:  ");
    Serial.println(range);
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_RANGE = 1;
    
    while( out_RANGE > 0){
      if(Serial.available() > 0){
          delay(100);
          range = branje(data);
          Serial.print("vrednost izbora:  ");
          Serial.print(range);
          if(range < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_RANGE = 0;}
       };
  };
};
void DEKADA(void){
   Serial.print("Trenutna vrednost dekade:  ");
    Serial.println();
    Serial.println("Vpisi novo zeljeno vrednost:");
    int out_DEKADA = 1;
    
    while( out_DEKADA > 0){
      if(Serial.available() > 0){
          delay(100);
          DekadRazred = branje(data);
          Serial.print("vrednost dekade:  ");
          Serial.print(DekadRazred);
          if(DekadRazred < 0){
            Serial.println("Ponovno vpisi vrednost, mora biti pozitivna!");
          }else{out_DEKADA = 0;}
       };
  };
};




///////////////////Branje vrednosti preko Serial.read\\\\\\\\\\\\\\\\\\\\\\\\

double branje(double j){
    int stev[10]={0,0,0,0,0,0,0,0,0,0};
    int i = 0;
    int tmp = 0;
    double del = 1;
    int del_tmp = 0;
    int del_konec_tmp = 0;
    double predznak = 1;
    double stevilo = 0;
    
    while(Serial.available() > 0){//dokler so informacije na voljo, toliko casa beremo--> eno crko na enkrat
      delay(20);
      tmp = Serial.read();            // 0 = 48, 9 = 57,    ,=44 in -=45
      if(tmp > 47){            //stevilo 0-:-9
            stev[i] = tmp-48; 
            i++;
          }else if(tmp == 44 || tmp ==46){   
              if(i <1)break;         //ce je vejica prvi znak, prekinemo ukaz branja, vrnemo 0
              del_tmp = i;           //decimalni index vejice
          }else { predznak = -1;};   //ce imamo negativno vrednost
    };////////sedaj imamo vsa zapisana stevila, decimalno vejico in predznak
    del_konec_tmp = i;      //shranimo podatek koliko je bilo vseh podatkov+1--> uporabimo pri postavitvi decimalne vejice
    ///////sedaj zdruzimo dobljene poodatke\\\\\\\\\\\\\\\\
    for(i = 0; i < del_konec_tmp; i++ ){
    stevilo = stevilo*10 + stev[i];
    };
    if(j == 1){    korekPr_vred = stevilo;
    }else if(j == 2){ pid_p_gain_vred = stevilo;
    }else if (j == 3){pid_i_gain_vred = stevilo;
    }else if(j == 4){pid_d_gain_vred = stevilo;
    }if(j == 5){  pid_max_vred = stevilo;};
    /////////preverimo decimalno vejico
    if(del_tmp >0){
      for(i = 0;i <(del_konec_tmp - del_tmp);i++){ del = del*10;};
      stevilo = stevilo/del;
    };
    
    if(j == 1){  korekPr_D = del;
    }else if(j == 2){ pid_p_gain_D = del;
    }else if (j == 3){pid_i_gain_D = del;
    }else if(j == 4){pid_d_gain_D = del;
    }if(j == 5) { pid_max_D = del;}
    
    if(predznak < 0){stevilo = stevilo*(-1);}  

  return stevilo;
};
//////////////////// Branje Potenciometra --> hitro&fino nastavljanje izbrane vrednosti tekom delovanja \\\\\\\\\\\\\
void potencM(void){// branje s potenciometra--> odvisno za katero vrednost imamo izbrano, za to vrednost odcitujemo s potenciometra
  if(izbor !=0 ){
        //potencVred = analogRead(A4)/del_1 * range;      //po testiranju, deli s toliko da dobis 1, nato se pomnozi s range--> da dobis do katere vrednosti bo kazalo
        if(potencVred < 0 )potencVred = 0;
        switch (izbor){  //kam zapisemo prebrano vrednost
         case 1: 
              korekPr_D = DekadRazred; 
              korekPr_vred = potencVred;
              break;
        case 2:
              pid_p_gain_D = DekadRazred;
              pid_p_gain_vred = potencVred;
              break;
        case 3:
              pid_i_gain_D = DekadRazred;
              pid_i_gain_vred = potencVred;
              break;
        case 4:
              pid_d_gain_D = DekadRazred;
              pid_d_gain_vred = potencVred;
              break;
        case 5:
             pid_max_D = DekadRazred;
             pid_max_vred = potencVred;
             break;
        }
  }
};

///////////////////Trenutne razmere koeficientov //////////////////
void displ(void){  
   
        Serial.println("Trenutne razmere koeficientov:");
        Serial.print("korekPr_SZ:  ");
        Serial.print(korekPr_SZ);
        Serial.print("  pid_p_gain_roll: ");
        Serial.print(pid_p_gain_roll);
        Serial.print("  pid_i_gain_roll: ");
        Serial.print(pid_i_gain_roll);
        Serial.print("  pid_d_gain_roll: ");
        Serial.print(pid_d_gain_roll);
        Serial.print("  pid_max_roll: ");
        Serial.println(pid_max_roll);
        Serial.println("");
        ////////Sklic naslednih ukazov\\\\\\\\\\\\\\\\\\\\\\\\\\\
        Serial.print("Popravek koeficientov:  ");
        Serial.print("korekPr_SZ-- 1   ");
        Serial.print("pid_p_gain_roll-- 2  ");
        Serial.print("pid_i_gain_roll-- 3  ");
        Serial.print("pid_d_gain_roll-- 4  ");
        Serial.print("pid_max_roll-- 5  ");
        Serial.print("kalibPotenc--6  ");
        Serial.println("konec-- 0 ");

}
