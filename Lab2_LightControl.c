void second_move(int led, int base, int times, int decrease){
    int i;
    for(i = 0; i < times; i++){
        GPIO_PTB_GPIO = led;
        delay1(150000);
        led /= 2;
        led -= decrease;
        led += base;
    }
}

int main()
{
    unsigned int i;

    OS_PowerOnDriverInitial();

    DRV_Printf("====================================\r\n", 0);
    DRV_Printf("   ADP-WT58F2C9 LED demo program   \r\n", 0);
    DRV_Printf("====================================\r\n", 0);

    DRV_Printf("LED testing ...\r\n", 0);

    GPIO_PTB_DIR = 0x0000;        //for GPIO
    GPIO_PTB_CFG = 0xffff;        //for open drain
    //GPIO_PTB_PADIN= 0x0000;       //for Digital Output

    float time = 15000;

// 0/1: LED on/off

    // First transformation
    for (i=0; i<6; i++){
            GPIO_PTB_GPIO = 0b1111111011111110;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011111101;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011111011;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011110111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011101111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011011111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111010111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111001111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111110101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111101101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111011101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1110111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1101111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1011111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b0111111101111111;
            delay1(time);
            //reverse
            GPIO_PTB_GPIO = 0b1011111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1101111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1110111101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111011101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111101101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111110101111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111001111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111010111111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011011111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011101111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011110111;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011111011;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011111101;
            delay1(time);
            GPIO_PTB_GPIO = 0b1111111011111110;
            delay1(time);
            time *= 1.5;
        }

    // Second transformation
     int led2 = 0b1111111011111110;
     int base = 0b1000000000000000;
     int decrease = 1;
     int movement = 13;

     second_move(led2, base, 7, decrease);

     led2 = 0b0111111111111100;
     for(i = 0 ; i < 15 ; i++){
         decrease *= 2;

         base = 0b1000000000000000;

         second_move(led2, base, movement, decrease);
         movement--;

         led2 -= decrease * 2;
     }

    // Third transformation
    for(i = 0; i < 20; i++){
        GPIO_PTB_GPIO = 0b0101010101010101;
