/* 
 * File:   LCD.h
 * Author: Dominik
 *
 * Created on June 8, 2015, 9:46 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef FCY
#define FCY 60000000
#endif

#include <xc.h>
#include <libpic30.h>

/*******************************************************************************
 * If you want to change the SPI module being used or the SPI pinds, please
 * modify ConfigSPI
*******************************************************************************/

#define SPIBUF      SPI2BUF

#define SET_RS      LATBbits.LATB13 = 1;
#define CLEAR_RS    LATBbits.LATB13 = 0;
#define CONFIG_RS   TRISBbits.TRISB13 = 0;

//normally, SS is active low, but in the LCD board, we need active high
#define ACTIVATE_CHIP_SELECT    LATBbits.LATB14 = 1;
#define RELEASE_CHIP_SELECT     LATBbits.LATB14 = 0;
#define CONFIG_CHIP_SELECT      TRISBbits.TRISB14 = 0;


//RPI46 =   25  =   EN / CS (active high) RB16
//RPI45 =   24  =   RS
//RPI44 =   23  =   SDI
//RP32  =   22  =   SDO
//RP42  =   21  =   SCK

    

/*******************************************************************************
 * Do not modify any #defines below this point. This are LCD module specific
*******************************************************************************/



#define LCD_CMD_CLEAR_DISPLAY           0b00000001  //clears the screen
#define LCD_CMD_RETURN_HOME             0b00000010  //returns the cursor to position 0
#define LCD_CMD_RETURN_LINE2            0b11000000  //moves the cursor to the start of line 2
#define LCD_CMD_ENTRY_MODE_INC_NO_S     0b00000110  //Increment, no display shift
#define LCD_CMD_ENTRY_MODE_DEC_NO_S     0b00000100  //Decrement, no display shift
#define LCD_CMD_ENTRY_MODE_INC_S        0b00000111  //Increment, display shift
#define LCD_CMD_ENTRY_MODE_DEC_S        0b00000101  //Decrement, display shift
#define LCD_CMD_DISPLAY_DON_CON_BON     0b00001111  //Display on, cursor on, blink on
#define LCD_CMD_DISPLAY_DON_CON_BOFF    0b00001110  //Display on, cursor on, blink off
#define LCD_CMD_DISPLAY_DON_COFF_BON    0b00001101  //Display on, cursor off, blink on
#define LCD_CMD_DISPLAY_DON_COFF_BOFF   0b00001100  //Display on, cursor off, blink off
#define LCD_CMD_DISPLAY_DOFF_CON_BON    0b00001011  //Display off, cursor on, blink on
#define LCD_CMD_DISPLAY_DOFF_CON_BOFF   0b00001010  //Display off, cursor on, blink off
#define LCD_CMD_DISPLAY_DOFF_COFF_BON   0b00001001  //Display off, cursor off, blink on
#define LCD_CMD_DISPLAY_DOFF_COFF_BOFF  0b00001000  //Display offs, cursor off, blink off
#define LCD_CMD_FUNCTION_8B_2L_FBIG     0b00111100  //8bit communiucation, 2 lines, 5x10 font
#define LCD_CMD_FUNCTION_8B_2L_FSML     0b00111000  //8bit communiucation, 2 lines, 5x8 font
#define LCD_CMD_FUNCTION_8B_1L_FBIG     0b00110100  //8bit communiucation, 1 line, 5x10 font
#define LCD_CMD_FUNCTION_8B_1L_FSML     0b00110000  //8bit communiucation, 1 line, 5x8 font
#define LCD_CMD_FUNCTION_4B_2L_FBIG     0b00101100  //4bit communiucation, 2 lines, 5x10 font
#define LCD_CMD_FUNCTION_4B_2L_FSML     0b00101000  //4bit communiucation, 2 lines, 5x8 font
#define LCD_CMD_FUNCTION_4B_1L_FBIG     0b00100100  //4bit communiucation, 1 line, 5x10 font
#define LCD_CMD_FUNCTION_4B_1L_FSML     0b00100000  //4bit communiucation, 1 line, 5x8 font
#define LCD_CMD_SET_DDRAM_ADDRESS       0b10000000  //bits 6-0 are used for the location
                                                    //line 1:   00....0F
                                                    //line2:    40....4F
/*******************************************************************************
  * Function Prototypes
*******************************************************************************/


#define LCD_ERROR_INVALID_DDRAM_ADDRESS -1000

int LCD_ConfigSPI(void);
int LCD_InitiaizeLCD(void);
int LCD_WriteStringNullTerm(char * string);
int LCD_WriteBuffer(char * buffer, char length);
int LCD_WriteCommand(char byte);
int LCD_WriteData(char byte);
int LCD_SetPosition(char row, char column);
int LCD_TEST(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

