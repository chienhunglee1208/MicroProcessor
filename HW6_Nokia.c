NOKIA Notes in pwm.h
unsigned short nokia_music[][2] = {
                       {NOTE_E6,2},{NOTE_D6,2},{NOTE_FS5,4},{NOTE_GS5,4},{NOTE_CS6,2},{NOTE_B6,2},
                       {NOTE_D5,4},{NOTE_E5,4},{NOTE_B5,2},{NOTE_A5,2},{NOTE_CS5,4},{NOTE_E5,4},
                       {NOTE_A5,5},{pause,2}
               };

duration *= 0.7;

main.c
#define REC_SIZE 1000
unsigned int rec_flag = 0, rec_index = 0;
unsigned int volume = 64;
int base = 28;
unsigned short rec_notes[REC_SIZE] = { 0 };
unsigned short notes[] = {
        NOTE_C0, NOTE_D0, NOTE_E0, NOTE_F0, NOTE_G0, NOTE_A0, NOTE_B0,
        NOTE_C1, NOTE_D1, NOTE_E1, NOTE_F1, NOTE_G1, NOTE_A1, NOTE_B1,
        NOTE_C2, NOTE_D2, NOTE_E2, NOTE_F2, NOTE_G2, NOTE_A2, NOTE_B2,
        NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3,
        NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4,
        NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5,
        NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6,
        NOTE_C7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_G7, NOTE_A7, NOTE_B7
};

void rec(){
    if(!rec_flag){
        int i;
        rec_flag = 1;  // start rec
        rec_index = 0;
        for(i = 0 ; i < REC_SIZE ; i++) rec_notes[i] = 0; // clear rec_notes
    }else rec_flag = 0;             // stop rec
}

void play_note(unsigned int key){
    unsigned int duration = ONE_SECOND * 60 / 96 / 4;
    GPIO_PTC_FS = 0xF000;     //for PWM Function

    PWM_EN = 0x000;         //Disable all PWM function

    PWM_BAS_CLK = 0x3;         //2MHz
    PWM_CLK8 = 0x9;         //128,0x0~0x3F --> PRIOD2 init value 200000
    PWM8 = 0x010;     //Duty Cycle,0x000~0xFFF half of period value
    PWM_EN = 0x100; //PWM8

    PERIOD_8 = 200000 / notes[key + base];
    PWM8 = (int) PERIOD_8 / volume;
    delay1( duration * 2 );

    PWM8 = 0;
    delay1( ONE_SECOND / 100 );
}

void control_notes(unsigned int key){
    switch(key){
        case 12: // high
            base -= 7;
            if(base < 0) base = 0;
            break;
        case 13: // mid
            base = 28;
            break;
        case 14: // low
            base += 7;
            if(base > 49) base = 49;
            break;
    }
}

void play_rec(){
    unsigned int duration = ONE_SECOND * 60 / 96 / 4 * 0.7;
    int i;

    // (1) GPIO Port C is used for PWM function
    GPIO_PTC_FS = 0xF000;     //for PWM Function

    PWM_EN = 0x000;         //Disable all PWM function

    PWM_BAS_CLK = 0x3;         //2MHz
    PWM_CLK8 = 0x9;         //128,0x0~0x3F --> PRIOD2 init value 200000
    PWM8 = 0x010;     //Duty Cycle,0x000~0xFFF half of period value
    PWM_EN = 0x100; //PWM8
    for (i = 0 ; i < rec_index ; i++){
        PERIOD_8 = 200000 / rec_notes[i];
        PWM8 = (int) PERIOD_8 / volume;
        delay1( duration * 3 );

        PWM8 = 0;
        delay1( ONE_SECOND / 100 );
    }
    delay1(2000000);
}

void demo_music(){
    unsigned int duration = ONE_SECOND * 60 / 96 / 4 * 0.7;
    int i;

    // (1) GPIO Port C is used for PWM function
    GPIO_PTC_FS = 0xF000;     //for PWM Function

    PWM_EN = 0x000;         //Disable all PWM function

    PWM_BAS_CLK = 0x3;         //2MHz
    PWM_CLK8 = 0x9;         //128,0x0~0x3F --> PRIOD2 init value 200000
    PWM8 = 0x010;     //Duty Cycle,0x000~0xFFF half of period value
    PWM_EN = 0x100; //PWM8

    for(i = 0 ; i < sizeof nokia_music/sizeof nokia_music[0]; i++) {
        PERIOD_8 = 200000 / nokia_music[i][0];
        PWM8 = (int) PERIOD_8 / volume;
        delay1( duration * nokia_music[i][1] );

        PWM8 = 0;
        delay1( ONE_SECOND/100 );
    }
    delay1(2000000);
}

int main()
{
    unsigned int key, col, tmp = 0;

    OS_PowerOnDriverInitial();

    while(1){
        key = 0xFF;
        GPIO_PTA_DIR = 0x0FF0;                        // cofigure pa0 ~ pa3 as output pins, pa4 ~ pa11 as input pins
        GPIO_PTA_CFG = 0x0000;
        for (col=0; col<4; col++)
        {
            GPIO_PTA_BS = 0x000F;                        // set key_0 ~ key_3 to 1
            GPIO_PTA_BR = 0x0000 | (1 << col);         // clear key_0 (col=0) ~ key_3 (col=3)
            tmp = ((~GPIO_PTA_PADIN) & 0xFF0) >> 4;    //  cache the status of push-buton, if pushed, 0 is read, if not pushed, 1 is read
            if (tmp > 0)
            {
                if (tmp & 0x1)                // check if bit 0 (b0) is 1 (true) or not (false)
                    key = 0*4 + col;
                else if (tmp & 0x2)            // check if bit 1 (b1) is 1 (true) or not (false)
                    key = 1*4 + col;
                else if (tmp & 0x4)
                    key = 2*4 + col;
                else if (tmp & 0x8)
                    key = 3*4 + col;
                else if (tmp & 0x80){ // sw18: control rec
                    rec();
                    delay1(500000);
                }else if (tmp & 0x20 && rec_index != 0) // sw20: play rec
                    play_rec();
                else if (tmp & 0x10) // sw21: play music
                    demo_music();
                break;
            }
        }
        if (key != 0xFF){
            if (key <= 6){
                play_note(key);
                if(rec_flag) rec_notes[rec_index++] = notes[key + base];
            }else if(key > 11 && key <= 14){
                control_notes(key);
            }
            delay1(200000);
        }
    }
    return 0;
}

