int abs(int n){
    if(n < 0)
        return n * (-1);
    return n;
}

int main()
{
    OS_PowerOnDriverInitial();

    // Setting for 7LED select
    GPIO_PTA_DIR = 0x0000;
    GPIO_PTA_CFG = 0x0000;
    GPIO_PTA_GPIO = Digit_1;

    // Setting for 7LED number
    GPIO_PTD_DIR = 0x0000;
    GPIO_PTD_CFG = 0x0000;
    GPIO_PTD_GPIO = 0xFFFF;

    GPIO_PTC_FS = 0x0000;
    GPIO_PTC_PADINSEL = 0x0000;
    GPIO_PTC_DIR = 0xFFFF;
    GPIO_PTC_CFG = 0x0000;

    // first: 40426233
    // second: 40426210
    unsigned int person[2][8] = {
            {4, 0, 4, 2, 6, 2, 3, 3},
            {4, 0, 4, 2, 6, 2, 1, 0}
    };
    unsigned int firstSwitch = 0x81, secondSwitch = 0x82, dipSwitch;
    unsigned int digits[8] = {
            Digit_1, Digit_2, Digit_3, Digit_4,
            Digit_5, Digit_6, Digit_7, Digit_8
    };
    unsigned int numbers[10] = {
            Number_0, Number_1, Number_2, Number_3, Number_4,
            Number_5, Number_6, Number_7, Number_8, Number_9
    };
    int i, j, index, remain;

    while(1){
        dipSwitch = (GPIO_PTC_PADIN >> 2) & 0x000000FF;
        if(dipSwitch == firstSwitch){
            int ledBase = 7;
            for(i = 0, index = 7 ; i < 8 ; i++, index--){
                for(remain = 0 ; remain < 1000 ; remain++){
                    for(j = i ; j >= 0 ; j --){
                        GPIO_PTD_GPIO = 0x0000;
                        GPIO_PTA_GPIO = digits[ledBase - j];
                        GPIO_PTD_GPIO = numbers[person[0][j + index]];
                    delay1(100);
                    }
                }
                delay1(100 * (7 - i));
            }
        }

        if(dipSwitch == secondSwitch){
            for(i = 0; i < 8 ; i++){
                for(remain = 0 ; remain < 100000 ; remain++){
                    for(j = i ; j >= 0 ; j--){
                        GPIO_PTD_GPIO = 0x0000;
                        GPIO_PTA_GPIO = digits[j];
                        GPIO_PTD_GPIO = numbers[person[1][abs(j - i)]];
                        delay1(500);
                    }
                }
                delay1(10000);
            }
        }

        delay1(500000 *2);
        for(i = 0 ; i < 8 ; i ++){
            GPIO_PTD_GPIO = 0x0000;
            GPIO_PTA_GPIO = digits[i];
        }
    }
    return 0;
}

