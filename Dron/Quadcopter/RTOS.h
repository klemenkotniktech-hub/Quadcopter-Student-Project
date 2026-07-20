//-----------------------------RTOS  Definicija-------------------------------------------------------------
#define TASK_COMPLETE 0
#define TASK_RUNNING 1





//----------------------------------------------------------------------------------------------------------
//definicija strukture rtos:
typedef void(*ptr_func)(void);  /* podatkovni tip; kazalec na funk */

struct _task {      //to je sedaj smatran kot nek tip spremenljivke
    ptr_func function;
    unsigned long last_tick;
    unsigned long priority;
    unsigned long interval;
};

typedef struct _task rtos_task_t;

//------------------------- Inicializacija katere funkcije bomo uporabili v Schedulerju----------------
extern rtos_task_t task_STANJE1;        // merjenje kotov
extern rtos_task_t task_STANJE2;        // merjenje pospeskometrov in poracunanje novih kotov
extern rtos_task_t task_STANJE3;        // merjenje compasa in poracunanje kota okoli Z-osi
extern rtos_task_t task_ANTENA;	        //extern--> pomeni da nekje obstaja ta funkcija, a ne v tem fajlu     povecas uro na vsako sekundo
extern rtos_task_t task_REG_MOT;        //isti funkciji tipki, le da omogocimo dovolj taskov za lep casovni slice
                                        // da pride task_URA po dolocenih ciklih vsako sekundo na vrsto
                                        // kombinacija regulacije in motorji
                                        //------------------------ Inicializacija seznama, vrstni red kako bodo nastopale funkcije(ce ne uporabimo prioritetnega razvrscanja)






rtos_task_t *rtos_task_list[] = { &task_STANJE1, &task_STANJE2, &task_STANJE3, &task_ANTENA, &task_REG_MOT }; //lazje je delati s kazalci,  ker manj podatkov naokoli prenasamo, namesto da bi celotno strukturo naokoli pelali
unsigned long rtos_num_task = sizeof(rtos_task_list) / sizeof(rtos_task_list[0]);


//------------------ Inicializacija Schedulerja, le inicializacija, nato ga z ENABLE se zazenes------------------------
unsigned long rtos_init(void) {      //dodatno lahko spremenis interval interrupta, a moras na roko poracunat,trenutno imam 4992microsekund dolg interval
                                     // https://www.instructables.com/id/Arduino-Timer-Interrupts/

    cli();//stop interrupts

          //set timer0 interrupt at 500Hz ali perioda= 2000 microsekund  
    TCCR0A = 0;// set entire TCCR2A register to 0
    TCCR0B = 0;// same for TCCR2B
    TCNT0 = 0;//initialize counter value to 0

              // set compare match register
    OCR0A = 110; // 120;    // 110;// = (16*10^6) / (563.636*256) - 1 (must be <256)  //110 --> 1774.1935 microsekund traja en task!
    // 115  ... x = 1855.61386  *5 = 9278.0693181 -> /10^-6 = 0.0092780693  --> 107.7810 Hz
    // 110  ... 1774.935 micro sekund

                    // turn on CTC mode
    TCCR0A |= (1 << WGM01);

    // Set CS02  bits for 256 prescaler    // v drugih kodah imas drug prescala, + narobe komentar oznacen
    TCCR0B |= (1 << CS02);

    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    return 0;
}

void rtos_enable(void) {
    TCNT0 = 0;        // ce preneha delovati, zbrisi to in spremeni zaporedje opravil v scedulerju(zacni z anteno)
    sei();            // dovoli interupte
}

void rtos_disable(void) {
    cli();            // ne dovoli interupte
}
//-------------------------------------------Scheduler-------------------------------------------------------------
ISR(TIMER0_COMPA_vect) { //na 2000 micro-sekund se sprozi
    interrupts();  // dovoljuje interrupt znotraj interrupta, tako lahko sedaj klicem STANJE in INTERRUPT1
    static int task_state = TASK_COMPLETE;

    if (0 == flag_ERROR)
    {

        if (task_state != TASK_COMPLETE) {
            // izkljuci vse motorje
            pretvorbaVrednosti(0, 0, 0, 0);
            Mot_SD.writeMicroseconds(val_Mot_SD);
            Mot_ZD.writeMicroseconds(val_Mot_ZD);
            Mot_ZL.writeMicroseconds(val_Mot_ZL);
            Mot_SL.writeMicroseconds(val_Mot_SL);

            rtos_disable();

            // POSTAVIMO FLAG O NAPAKI NA 1
            flag_ERROR = 1;
            flag_stanje = stanje_ERROR;   // tu moramo prepisati stanje, saj se bo ta pobrisal, ko bomo izstopili iz problematicne rezine

            // stanje: 0->vse ok, 1-> stanje1, 2-> stanje2 , 3-> stanje3, 4->antena, 5-> regulacija, 6,7,8-> motorji
            Serial.print("Stanje: ");
            Serial.print(stanje_ERROR);
            Serial.println("  Casovna rezina predolga...  ");

            rtos_enable();
            // tukaj bom sedaj scheduler zaciklav v while zanki, tega ne bo mogel zapustiti
            // hkrati deluje scheduler, tako da bo ponovno klical rezine, a sedaj bomo 
            Error_loop();       // TU NOTRI SE BO PROGRAM ZACIKLAV, a hkrati mora povecevati stevec za LEDico
        }
        else
        {
            task_state = TASK_RUNNING;
            static unsigned long i = 0;
            rtos_task_list[i]->function();  //poklice izbrano opravilo
            i++;
            if (i == rtos_num_task) {
                i = 0;
            }
            stanje_ERROR = i + 1;   // i se steje od 0
            task_state = TASK_COMPLETE;
        }
    }
    else
    {
        // povecas stevec countra -> se uporabi za dolocevanje casa -> LEDico
        LED_counter_increase(); 

    }
}
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

///////////////////////////// DODANA FUNKCIJA \\\\\\\\\\\\ ///
///////////////////////////////////////////////////////////////
void Error_loop(void)
{
    int ponovitev;
    while (1)   // preidemo v neskoncni loop, kjer prenehamo z vsemi aktivnimi dejavnostmi!!!
    {
        // odvisno od 'flag_stanje', tolikokrat bo utripnila LEDica, nato 1sekundo nic
        // casovno rezinjenje periode je priblizno na 100Hz -> (5*rtos_num_task = 1 perioda)
        for (ponovitev = flag_stanje; ponovitev > 0; ponovitev--)
        {
            // 0,2 sekundi sveti
            digitalWrite(LED, HIGH);
            counter_goal = 20;      // nastavimo koliko period mora miniti (5*rtos_num_task = 1 perioda)
            flag_LED_uspeh = 1;
            while (flag_LED_uspeh) // cakamo na stevec da resetira flag
            {
                Serial.print("Vklop -- counter_rezina_LED: ");
                Serial.print(counter_rezina_LED);
                Serial.print("  counter_perioda_LED: ");
                Serial.println(counter_perioda_LED);
            };
            
                                        // 0,2 sekundi ne sveti
            digitalWrite(LED, LOW);
            counter_goal = 20;      // nastavimo koliko period mora miniti (5*rtos_num_task = 1 perioda)
            flag_LED_uspeh = 1;
            while (flag_LED_uspeh) // cakamo na stevec da resetira flag
            {
                Serial.print("Izklop -- counter_rezina_LED: ");
                Serial.print(counter_rezina_LED);
                Serial.print("  counter_perioda_LED: ");
                Serial.println(counter_perioda_LED);
            };
        }
        // sedaj 1sekundo ne sveti
        counter_goal = 100;      // nastavimo koliko period mora miniti (5*rtos_num_task = 1 perioda)
        flag_LED_uspeh = 1;
        while (flag_LED_uspeh) // cakamo na stevec da resetira flag
        {
            Serial.print("Pavza -- counter_rezina_LED: ");
            Serial.print(counter_rezina_LED);
            Serial.print("  counter_perioda_LED: ");
            Serial.println(counter_perioda_LED);
        };
    }
}

void LED_counter_increase(void)
{
    // tukaj bos poveceval counter, s katerim bom nato kontroliral utripanje LEDice
    if (1 == flag_LED_uspeh)
    {
        counter_rezina_LED = counter_rezina_LED + 1;
        if (rtos_num_task == counter_rezina_LED)
        {
            counter_rezina_LED = 0;                         // resetiramo stevec
            counter_perioda_LED = counter_perioda_LED + 1;  // povecamo stevec periode, ta je kljucen za LEDico

                                                            // preverimo ce je stevec periode enak zeleni vrednosti period 

            if (counter_perioda_LED == counter_goal)
            {
                flag_LED_uspeh = 0;      // counter_rezina_LED == counter_goal
                counter_perioda_LED = 0;
            }
        }
    }
}