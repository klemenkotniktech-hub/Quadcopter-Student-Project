

void LCD_N(double vred_LCD_N[6]) {

    // zgornja vrstica 
    lcd.setCursor(0, 0);
    lcd.print(((int)vred_LCD_N[0]));
    lcd.setCursor(5, 0);
    lcd.print((vred_LCD_N[1]));
    lcd.setCursor(11, 0);
    lcd.print((vred_LCD_N[2]));


    // spodnja vrstica   
    lcd.setCursor(0, 1);
    lcd.print((vred_LCD_N[3]));    // maks vrednost
    lcd.setCursor(5, 1);
    lcd.print((vred_LCD_N[4]));
    lcd.setCursor(11, 1);
    lcd.print((vred_LCD_N[5]));

};

////////////////////// LCD \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void LCD_S(char vred_LCD_C[],int stolpec, int vrstica)
{
    // zgornja vrstica 
    lcd.setCursor(stolpec, vrstica);
    lcd.print((vred_LCD_C));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_show(void) {

    lcd.clear();
    double vred_LCD_N[6];
    vred_LCD_N[0] = (int)pogon;
    vred_LCD_N[1] = Izklop_pogona;
    vred_LCD_N[2] = korekcijaSZ_Xos;
    vred_LCD_N[3] = pid_p_gain_roll;
    vred_LCD_N[4] = pid_i_gain_roll;
    vred_LCD_N[5] = pid_d_gain_roll;
    LCD_N(vred_LCD_N);
};