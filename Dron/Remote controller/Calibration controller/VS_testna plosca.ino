//koda daljinca: izvedba meritev, nato oddaja podatkov, ni casovnega rezinjenja
//               normalno deluje(brez prekinitev), ker je tako enostavneje...
// Imel bom 6 potenciometrov za: P, I in D gain za X- in Y-os,+ nastavljanje pogona (Z-os bom naknadno). 
//  Analog: 0-pogon; 1- korek_SZ; 2- PID P_gain; 3- PID I_gain; 4- PID D_gain; 5- PID maks

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(8, 9);                // spremenil sem vhodna pina: CE--8  in CSN--9
const byte rxAddr[6] = { 00001 };  //"00001";

#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);  /// POZOR!!!! \\\\\\\ originalno je tu: lcd(12, 11, 5, 4, 3, 2); prve dve zici sta zamenjani

#include "Inicializacija.h"

                                      /////////////////////////// casovna analiza \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                                      double timeS = 0;
double timeK = 0;
double timeR = 0;

void setup() {/////////////////////////////////////////
    
    Inicializacija_Turn_off_switch();

    //  sprozi, ko bos zacel delati z anteno
    radio.begin();
    radio.setRetries(15, 15);
    radio.openWritingPipe(rxAddr);
    radio.stopListening();

    Serial.begin(9600);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    //////////////////// 16 push button matrix -> nakoncu bos izbral kateri pini bodo veljavni-> veljavna 4 gumbi S1, S2, S5 in S6
    ///////////////////// S1 --> blokiranje sistema--> pogon = 0!!!!!   S2 --> aktivira nazaj sistem  (upostevajoc potenciometre)
    ////////////////////S S2 --> 

}/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////// MAIN FUNCTION   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


void loop() {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// Spreminjanje vrednosti potenciometrov in njihovega pomena--> push button \\\\\\\\\\\\\\\\\\\\\\
    
    /*
    double H = (analogRead(A2)+500)/1023.0;
    Serial.print("SZ_Xos");
    Serial.println(H);
    Koef_spreminjanje();
    */

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////// Branje potenciometrov in S1 ter S2 (levo  &  desno) \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    
    potencRead();           // v vsakem primeru preberemo vrednosti, a jih v deaktivaciji ne bomo upostevali
    Turn_off_swithc();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
//// Preberi potenciometre in shrani vrednosti v LCD prikazovalno bazo-- vrstni red in raporeditev \\\\\\\\\\\\\\\\\\\\\\\\\\\
  
    if (dovoljenjeBranje == 1) {
        LCD_show();
    }
    else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print((int)pogon);
        char tekst[] = "Deaktivirano";
        int stolpec = 0;
        int vrstica = 1;
        LCD_S(tekst, stolpec, vrstica);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////// Antena in oddaja informacij \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //timeS = micros();
    antena_send();  
    //timeK = micros();
    //timeR = timeK-timeS;
    //Serial.println(timeR);

    Serial.print("Pogon   "); Serial.print(pogon);
    Serial.print("Pogon vred  "); Serial.println(pogon_vred);

}
