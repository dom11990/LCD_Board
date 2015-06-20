#include "LCD.h"

/**
 * Configures SPI module and Pins 21-25 for the LCD Board's use.
 * @return  0 on success
 */
int LCD_ConfigSPI(void)
{
    //All pins used by the module need to be set to DIGITAL by clearing
    //their respective ANSEL registers

    //If the device has a Peripheral Pin Select (PPS) feature, the SCKx pin must
    //be mapped as both input and output in Master mode

    //SPIxCON1 configuration
    SPI2CON1bits.DISSCK = 0b0;      //master mode
    SPI2CON1bits.DISSDO = 0;        //using SDO
    SPI2CON1bits.MODE16 = 0;        //communication is byte-wide
    SPI2CON1bits.SMP = 0;           //data is sampled at middle of data
                                    //output time
    SPI2CON1bits.CKE = 1;           //data changes when clock goes from idle to
                                    //active (in this config, low to high)
    SPI2CON1bits.SSEN = 0;          //this module is master, no need for SS
    SPI2CON1bits.CKP = 0;           //clock idle is low, active is high
    SPI2CON1bits.MSTEN = 1;         //module is in master mode
    SPI2CON1bits.SPRE = 0b111;      //1:1 secondary prescaler
    SPI2CON1bits.PPRE = 0b00;       //64:1 primary prescaler

    //SPIxCON2 configuration
    SPI2CON2bits.FRMEN = 0;         //Frame SPI support is disabled
    SPI2CON2bits.SPIFSD = 0;        //Frame sync pulse output (master)
    SPI2CON2bits.FRMPOL = 0;        //Frame sync pulse is active low
    SPI2CON2bits.FRMDLY = 0;        //Frame sync pulse coincides with first bit
    SPI2CON2bits.SPIBEN = 1;        //Enhanced buffer mode enabled

    //RB14  =   25  =   SS
    //RB13  =   24  =   RS
    //RPI44 =   23  =   SDI
    //RP32  =   22  =   SDO
    //RP42  =   21  =   SCK

    RPINR22bits.SDI2R = 0b0101100;  //RPI44 SDI
    RPOR4bits.RP43R = 0b001000;     //RP43 SDO
    RPINR22bits.SCK2R = 0b0101010;  //RP42 CLK in?
    RPOR4bits.RP42R = 0b001001;     //RP42 CLK out

    CONFIG_CHIP_SELECT
    CONFIG_RS

    SPI2STATbits.SPISIDL = 0;       //module continues operation in idle mode
    SPI2STATbits.SPIEN = 1;         //Enables the module

    return 0;
}

/**
 *
 * @return 0 on success
 */
int LCD_InitiaizeLCD(void)
{
    LCD_WriteCommand(LCD_CMD_CLEAR_DISPLAY);
    LCD_WriteCommand(LCD_CMD_FUNCTION_8B_2L_FSML);
    LCD_WriteCommand(LCD_CMD_DISPLAY_DON_COFF_BOFF);
    LCD_WriteCommand(LCD_CMD_ENTRY_MODE_INC_NO_S);
    LCD_WriteCommand(LCD_CMD_RETURN_HOME);

    return 0;
}

/**
 * Writes a string to the LCD, expect null terminator in the string ('\0')
 * @param string null terminated string to write to the LCD
 * @return 0 on success
 */
int LCD_WriteStringNullTerm(char * string)
{
    while(string)
    {
        LCD_WriteData(*string);
        string++;
    }

    return 0;
}

/**
 * Writes a sequence of chars to the display.
 * @param buffer the char[] to be written to the LCD
 * @param length number of elements to write
 * @return 0 on success
 */
int LCD_WriteBuffer(char * buffer, char length)
{
    while(length)
    {
        LCD_WriteData(*buffer);
        buffer++;
        length--;
    }

    return 0;
}

/**
 * Writes a command (RS = 0) to the LCD. Used for configuring various LCD
 * settings.
 * @param byte
 * @return 0 on success
 */
int LCD_WriteCommand(char byte)
{
    char temp;
    //byte = reverseByte(byte);
    ACTIVATE_CHIP_SELECT
    CLEAR_RS
    SPIBUF = byte;

    //wait for the shift operation to complete
    while(SPI2STATbits.SRXMPT)
        Nop();

    __delay_us(100);
    //read out the empty byte that is now sitting in the read buffer
    temp = SPIBUF;
    RELEASE_CHIP_SELECT
    __delay_ms(2);
    return 0;
}

/**
 * Writes to the data memory of the LCD (RS = 1). Use this function to write
 * text to be displayed.
 * @param byte
 * @return 0 on success
 */
int LCD_WriteData(char byte)
{
    char temp;

    ACTIVATE_CHIP_SELECT
    SET_RS
    SPIBUF = byte;

    //wait for the shift operation to complete
    while(SPI2STATbits.SRXMPT)
        Nop();

    __delay_us(100);
    //read out the empty byte that is now sitting in the read buffer
    temp = SPIBUF;
    RELEASE_CHIP_SELECT
    CLEAR_RS
            
    return 0;
}

/**
 * Sets the DDRAM address to the specified row and column. This is where text
 * will be written to.
 * @param row 0 indexed row
 * @param column 0 indexed column
 * @return 0 on success
 */
int LCD_SetPosition(char row, char column)
{
    char DDRAM_address;
    //for a 2x16 display:
    //row 0 is DDRAM 0x00 to 0x0F
    //row 1 is DDRAM 0x40 to 0x4F
    row = (row)?0x40:0x00;

    DDRAM_address = row+column;

    //only perform the LCD operation if the address is valid
    if(DDRAM_address <= 0x4F)
        LCD_WriteCommand(LCD_CMD_SET_DDRAM_ADDRESS|DDRAM_address);
    else
        return LCD_ERROR_INVALID_DDRAM_ADDRESS;
    return 0;
}


/**
 * Configures the SPI module, and writes some test text to the LCD.
 * Effectively tests the basic functionality of the display.
 * @return
 */
int LCD_TEST(void)
{
    LCD_ConfigSPI();
    LCD_InitiaizeLCD();
    LCD_WriteData('L');
    LCD_WriteData('i');
    LCD_WriteData('n');
    LCD_WriteData('e');
    LCD_WriteData(' ');
    LCD_WriteData('1');
    LCD_WriteCommand(LCD_CMD_RETURN_LINE2);
    LCD_WriteData('L');
    LCD_WriteData('i');
    LCD_WriteData('n');
    LCD_WriteData('e');
    LCD_WriteData(' ');
    LCD_WriteData('2');

    LCD_SetPosition(0,0xA);
    LCD_WriteData('T');
    LCD_WriteData('e');
    LCD_WriteData('x');
    LCD_WriteData('t');
    LCD_WriteData(' ');
    LCD_WriteData('1');

    LCD_SetPosition(1,0xA);
    LCD_WriteData('T');
    LCD_WriteData('e');
    LCD_WriteData('x');
    LCD_WriteData('t');
    LCD_WriteData(' ');
    LCD_WriteData('2');


    return 0;
}