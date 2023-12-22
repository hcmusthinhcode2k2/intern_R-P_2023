
//
//#include "libcomp.h"
#include "LCD_I2C.h"

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _numlines;
static uint8_t _backlightval;
static uint8_t colBk=0, rowBk=0;

static void LCD_I2C_pulseEnable(uint8_t _data) // <editor-fold defaultstate="collapsed" desc="EN pin high 1us -> low 50us">
{
    _data|=LCD_EN;
    LCD_I2C_writeNByte(LCD_I2C_Address, &_data, 1); // En high
    __delay_us(1); // enable pulse must be >450ns
    _data&=(~LCD_EN);
    LCD_I2C_writeNByte(LCD_I2C_Address, &_data, 1); // En low
    __delay_us(50); // commands need > 37us to settle
}// </editor-fold>

static void LCD_I2C_write4bits(uint8_t value) // <editor-fold defaultstate="collapsed" desc="Write 4 bit to pin">
{
    value|=_backlightval;
    LCD_I2C_writeNByte(LCD_I2C_Address, &value, 1);
    LCD_I2C_pulseEnable(value);
}// </editor-fold>

static void LCD_I2C_send(uint8_t value, uint8_t mode) // <editor-fold defaultstate="collapsed" desc="Write 8 bit to pin and mode">
{
    uint8_t highnib=value&0xf0;
    uint8_t lownib=(value<<4)&0xf0;
    LCD_I2C_write4bits((highnib)|mode);
    LCD_I2C_write4bits((lownib)|mode);
}// </editor-fold>

 void LCD_I2C_command(uint8_t value) // <editor-fold defaultstate="collapsed" desc="Send Command">
{
    LCD_I2C_send(value, 0);
}// </editor-fold>

 void LCD_I2C_clear(void) // <editor-fold defaultstate="collapsed" desc="Clear Display">
{
    LCD_I2C_command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    __delay_ms(2); // this command takes a long time!
}// </editor-fold>

 void LCD_I2C_home(void) // <editor-fold defaultstate="collapsed" desc="Set cursor position to zero">
{
    LCD_I2C_command(LCD_RETURNHOME);
    __delay_ms(2); // this command takes a long time!
}// </editor-fold>

 void LCD_I2C_setCursor(uint8_t col, uint8_t row) // <editor-fold defaultstate="collapsed" desc="Set cursor position">
{
    int row_offsets[]={0x00, 0x40, 0x14, 0x54};

    if(row>_numlines)
    {
        row=_numlines-1; // we count rows starting w/0
    }

    LCD_I2C_command(LCD_SETDDRAMADDR|(col+row_offsets[row]));
    colBk=col;
    rowBk=row;
}// </editor-fold>

 void LCD_I2C_display(void) // <editor-fold defaultstate="collapsed" desc="On Display Control">
{
    _displaycontrol|=LCD_DISPLAYON;
    LCD_I2C_command(LCD_DISPLAYCONTROL|_displaycontrol);
}// </editor-fold>

 void LCD_I2C_write(uint8_t value) // <editor-fold defaultstate="collapsed" desc="Write char to display">
{
    LCD_I2C_send(value, LCD_RS);
}// </editor-fold>

 void LCD_I2C_noBacklight(void) // <editor-fold defaultstate="collapsed" desc="Light Off">
{
    _backlightval=LCD_NOBACKLIGHT;
    LCD_I2C_writeNByte(LCD_I2C_Address, &_backlightval, 1);
}// </editor-fold>

 void LCD_I2C_backlight(void) // <editor-fold defaultstate="collapsed" desc="Light On">
{
    _backlightval=LCD_BACKLIGHT;
    LCD_I2C_writeNByte(LCD_I2C_Address, &_backlightval, 1);
}// </editor-fold>

 void LCD_I2C_backlightToggle(void) // <editor-fold defaultstate="collapsed" desc="Light Toggle">
{
    if(_backlightval==LCD_NOBACKLIGHT)
        _backlightval=LCD_BACKLIGHT;
    else
        _backlightval=LCD_NOBACKLIGHT;

    LCD_I2C_writeNByte(LCD_I2C_Address, &_backlightval, 1);
}// </editor-fold>

 void LCD_I2C_Init(uint8_t cols, uint8_t lines, uint8_t dotsize) // <editor-fold defaultstate="collapsed" desc="Initialize LCD">
{
    _displayfunction=LCD_4BITMODE|LCD_1LINE|LCD_5x8DOTS;

    if(lines>1)
    {
        _displayfunction|=LCD_2LINE;
    }
    _numlines=lines;

    // for some 1 line displays you can select a 10 pixel high font
    if((dotsize!=0) && (lines==1))
    {
        _displayfunction|=LCD_5x10DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    __delay_ms(50);

    // Now we pull both RS and R/W low to begin commands
    _backlightval=LCD_NOBACKLIGHT;
    LCD_I2C_writeNByte(LCD_I2C_Address, &_backlightval, 1);
    //	LCD_expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
    //__delay_ms(1000);
    __delay_ms(500);

    //put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    LCD_I2C_write4bits(0x03<<4);
    __delay_ms(5); // wait min 4.1ms

    // second try
    LCD_I2C_write4bits(0x03<<4);
    __delay_ms(5); // wait min 4.1ms

    // third go!
    LCD_I2C_write4bits(0x03<<4);
    __delay_us(150);

    // finally, set to 4-bit interface
    LCD_I2C_write4bits(0x02<<4);

    // set # lines, font size, etc.
    LCD_I2C_command(LCD_FUNCTIONSET|_displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol=LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;
    LCD_I2C_display();

    // clear it off
    LCD_I2C_clear();

    // Initialize to default text direction (for roman languages)
    _displaymode=LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    LCD_I2C_command(LCD_ENTRYMODESET|_displaymode);

    LCD_I2C_home();

}// </editor-fold>

 void LCD_I2C_print(const char *pD) // <editor-fold defaultstate="collapsed" desc="LCD print string">
{
    while(*pD!=0x00)
    {
        LCD_I2C_write(*pD);
        pD++;
    }

    LCD_I2C_setCursor(colBk, rowBk);
} // </editor-fold>