void Inicializacija_Turn_off_switch(void)
{
    pinMode(Switch_D_pin, INPUT_PULLUP);
    // ce bo stikalo izkluceno, bo vrednost = 1 (vse je prav), ce bo stikalo vkluceno je vrednost = 0 (izkluci pogon)
};

void Turn_off_swithc(void)
{
    Izklop_pogona = ! digitalRead(Switch_D_pin);  // negiram 
};