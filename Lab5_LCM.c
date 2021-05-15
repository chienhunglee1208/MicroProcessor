lcm.h
#ifndef LCM_H_INCLUDED
#define LCM_H_INCLUDED

#define CleanSet 0x0000

// GPIO Port E, DB0~DB7 (PE0~PE7), R/W (PE8), E (PE9), RS (PE10)
#define RW 0x0100   // read:1 or write:0  bit 8
#define E  0x0200   // execute (enable)      bit 9
#define RS 0x0400   // register select (1: data register or 0: instruction register) bit 10

// instruction to call lcd function  RS=0  RW=0 (Write Instruction Register)
#define LCD_ClearDisplay    0x01  // clear display buffer (DDRAM)
#define LCD_ReturnHome      0x02  // return cursor and display to original position
#define LCD_EntryMode       0x04  // Entry Mode Set,  I/D (Increment/decrement) S (Shift)
#define LCD_DisplayCursor   0x08  // Set Display(D) on/off and Cursor (C) on/off, B: cursor blinking
#define LCD_Shift           0x10  // display or cursor shift, S/C (1: display, 0: cursor), R/L (1: shift right, 0: shift left)
#define LCD_Function        0x20  // set DL (interface data length),  N (number of display lines), F (character font)
#define LCD_SetCGRAM        0x40  // create font
#define LCD_SetDDRAM        0x80  // cursor position setting

// RS=0  RW=1  Read Instruction Register
#define LCD_ReadBusyAddress 0x100  // read busy flag (BF) and address counter content, RS=0, RW=1

// RS=1  RW=0  Write data into CGRAM or DDRAM
#define LCD_WriteData       0x200  // RS=1, RW=0

// RS=1  RW=1  Read data from CGRAM or DDRAM
#define LCD_ReadData        0x300  // RS=1, RW=1

// flags
unsigned short LCD_ACShiftDirection = 0x02;   // I/D bit 1, 1: increment, 0: decrement
unsigned short LCD_DisplayAutoShift = 0x01;   // S   bit 0 (1: accompany display shift during data write and read)

unsigned short LCD_Display = 0x04;            // D bit 2 (display on)
unsigned short LCD_Cursor = 0x02;             // C bit 1 (cursor on)
unsigned short LCD_CursorBlink = 0x01;        // B bit 0 (cursor blinking)

unsigned short LCD_ShiftMode = 0x08;          // S/C bit 3 (1: display shift / 0: cursor move)
unsigned short LCD_ShiftDirection = 0x04;     // R/L bit 2 (1: shift right / 0: shift left)

unsigned short LCD_InstructionMode = 0x10;    // DL bit 4 (interface data length, 1: 8 bits, 0: 4bits)
unsigned short LCD_NumLine = 0x08;            // N  bit 3 (number of display lines, 1: 2 lines, 0: 1 line)
unsigned short LCD_FontSize = 0x04;           // F  bit 2 (character font size, 1: 5x10 dots, 0: 5x8 dots)

unsigned short LCD_BusyFlag = 0x80;            // BF bit 7 (busy flag, 1: internally operating, 0: instruction acceptable)


#endif // LCM_H_INCLUDED

main.c
unsigned int index1 = 0x0, index2 = 0x40;
void Display1(){
    char output[] = "Hello World";

    char i=0;
    //WriteIns(0x38);
    WriteIns(LCD_Function | LCD_InstructionMode | LCD_NumLine);

    //WriteIns(0x0E);
    WriteIns(LCD_DisplayCursor | LCD_Display | LCD_Cursor);  //DISPLAY CONTROL

    //WriteIns(0x06);
    WriteIns(LCD_EntryMode | LCD_ACShiftDirection);  // SET INPUT MODE

    // Shift DDRAM Address to 0xA
    WriteIns(LCD_SetDDRAM | index1);  //SET DDRAM Address

    for(i=0;i<strlen(output);i++){
        WriteData(output[i]);
        delay1(100000);
    }
}

void Display2(){
    char output[] = "NISRA Hackathon 18";

    char i=0;
    //WriteIns(0x38);
    WriteIns(LCD_Function | LCD_InstructionMode | LCD_NumLine);

    //WriteIns(0x0E);
    WriteIns(LCD_DisplayCursor | LCD_Display | LCD_Cursor);  //DISPLAY CONTROL

    //WriteIns(0x06);
    WriteIns(LCD_EntryMode | LCD_ACShiftDirection);  // SET INPUT MODE

    // Shift DDRAM Address to 0xA
    WriteIns(LCD_SetDDRAM | index2);  //SET DDRAM Address

    for(i=0;i<strlen(output);i++){
        WriteData(output[i]);
        delay1(100000);
    }
}

int main()
{
    unsigned int i, tmp = 0, key, col, cursor_switch;
    cursor_switch = LCD_DisplayCursor | LCD_Display | LCD_Cursor;
    GPIO_PTA_FS = 0x0000;
    GPIO_PTA_PADINSEL = 0x0000;

    OS_PowerOnDriverInitial();
    WriteIns(LCD_ClearDisplay);
    WriteIns(cursor_switch);


    DRV_Printf("====================================\r\n", 0);
    DRV_Printf("   ADP-WT58F2C9 LCM demo program   \r\n", 0);
    DRV_Printf("====================================\r\n", 0);


    DRV_Printf("LCM testing ...\r\n", 0);


    while(1){
        key = 0xFF;
        GPIO_PTA_DIR = 0x0FF0;                        // cofigure pa0 ~ pa3 as output pins, pa4 ~ pa11 as input pins
        GPIO_PTA_CFG = 0x0000;
        for (col=0; col<4; col++){
            GPIO_PTA_BS = 0x000F;                        // set key_0 ~ key_3 to 1
            GPIO_PTA_BR = 0x0000 | (1 << col);         // clear key_0 (col=0) ~ key_3 (col=3)
            tmp = ((~GPIO_PTA_PADIN) & 0xFF0) >> 4;    //  cache the status of push-buton, if pushed, 0 is read, if not pushed, 1 is read
            if (tmp > 0){
                if (tmp & 0x1)                // check if bit 0 (b0) is 1 (true) or not (false)
                    key = 0*4 + col;
                else if (tmp & 0x2)            // check if bit 1 (b1) is 1 (true) or not (false)
                    key = 1*4 + col;
                else if (tmp & 0x4)
                    key = 2*4 + col;
                else if (tmp & 0x8)
                    key = 3*4 + col;
                break;
            }
        }

        if (key != 0xFF){     // it means that some push-button is pushed,
            switch(key){  // 第幾個按鈕
                case 0:
                    Display1();
                    break;
                case 1:
                    Display2();
                    break;
                case 2:
                    delay1(200000);
                    WriteIns(LCD_Shift | LCD_ShiftDirection | LCD_ShiftMode);

                    break;
                case 3:
                    delay1(200000);
                    WriteIns(LCD_Shift | LCD_ShiftMode);
                    break;
                case 4:
                    WriteIns(LCD_ClearDisplay);
                    break;
                case 5:
                    WriteIns(LCD_ReturnHome);
                    break;
                case 6:
                    delay1(200000);
                    cursor_switch = cursor_switch ^ 0x02;
                    WriteIns(cursor_switch);
                    break;

            }
        }
    }


    InitialLCD();

    Display_1Line(10);
    delay1(1000000);
    WriteIns(0x01);  // clear buffer

    Display_2Line(5);
    delay1(1000000);
    WriteIns(0x01);  // clear buffer

    WriteIns(0x80);  //1-LINE DD RAM SET Address
    for(i=0;i<16;i++)
        WriteData(255);
    WriteIns(0xC0);  //2-LINE DD RAM SET Address
    for(i=0;i<16;i++)
        WriteData(255);

    DRV_Printf("====================================\r\n", 0);


    return 0;
}

