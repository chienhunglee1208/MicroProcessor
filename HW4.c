int main()
{
    unsigned int tmp = 0;

    OS_PowerOnDriverInitial();

    GPIO_PTA_FS = 0x0000;
    GPIO_PTA_PADINSEL = 0x0000;

    // Setting for 7LED select
    GPIO_PTA_DIR = 0x0000;
    GPIO_PTA_CFG = 0x0000;
    GPIO_PTA_GPIO = Digit_8;

    // Setting for 7LED number
    GPIO_PTD_DIR = 0x0000;
    GPIO_PTD_CFG = 0x0000;
    GPIO_PTD_GPIO = Number_8 | Number_Dot;

    GPIO_PTC_DIR = 0x1111;

    unsigned int col, i = 0, j, sign = 0;
    unsigned int remain, key, led = 7;
    unsigned int index_7LED_NUM[17] = {Number_0, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7,
            Number_8, Number_9, Number_A, Number_b, Number_C, Number_d, Number_E, Number_F, Number_Dash, };
    unsigned int led_index[8] = {
            Digit_1, Digit_2, Digit_3, Digit_4, Digit_5,
            Digit_6, Digit_7, Digit_8
    };
    unsigned int input_count = 0;
    int input[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    int answer[] = { 0xFF, 0xFF, 0xFF, 0xFF };

    while(1)
    {
        // Get DIP switch's value
        tmp = (GPIO_PTC_PADIN >> 2) & 0x000000FF;
        // this value depends on dip switch's situation
        DRV_Printf("%x\r\n", tmp);

        // if open DIP switch '1', reset.
        if( tmp == 0x0049 ){
            DRV_Printf("Reset\r\n", 0);
            i = 0;
            sign = 0;
            led = 7;
            input_count = 0;
            for (j = 0 ; j < 4 ; j++){
                input[j] = 0xFF;
                answer[j] = 0xFF;
            }
        }

        // get button input
        key = 0xFF;
        GPIO_PTA_DIR = 0x0FF0;
        GPIO_PTA_CFG = 0x0000;
        for (col=0; col<4; col++){
            GPIO_PTA_BS = 0x000F;                       // set key_0 ~ key_3 to 1
            GPIO_PTA_BR = 0x0000 | (1 << col);         // clear key_0 (col=0) ~ key_3 (col=3)
            tmp = ((~GPIO_PTA_PADIN) & 0xFF0) >> 4;    //  cache the status of push-buton, if pushed, 0 is read, if not pushed, 1 is read
            if (tmp > 0){
                if (tmp & 0x1)
                    key = 0*4 + col;
                else if (tmp & 0x2)
                    key = 1*4 + col;
                else if (tmp & 0x4)
                    key = 2*4 + col;
                else if (tmp & 0x8)
                    key = 3*4 + col;
                else if (tmp & 0x80)
                    sign = 1;
                else if (tmp & 0x40)
                    sign = 2;
                else if (tmp & 0x20)
                    sign = 3;
                else if (tmp & 0x10)
                    sign = 4;
                break;
            }
        }

        if (key != 0xFF && input_count < 4){     // it means that some push-button is pushed,
            // get 4 button input and store in input[]
            if(key + 1 > 9)
                input[input_count] = (key + 1) - 10;
            else
                input[input_count] = key + 1;
            input_count++;
        }else{
            // after input 4 number, wait for a sign input.
            int num1, num2, result;
            num1 = input[0] * 10 + input[1];
            num2 = input[2] * 10 + input[3];
            switch(sign){
                case 1:
                    result = num1 + num2;
                    break;
                case 2:
                    result = num1 - num2;
                    break;
                case 3:
                    result = num1 * num2;
                    break;
                case 4:
                    result = num1 % num2;
                    break;
            }

            // Deal with negative number.
            if(result >= 0){
                i = 3;
                while(result > 0){
                    answer[i--] = result % 10;
                    result /= 10;
                }
            }else{
                result *= -1;
                i = 3;
                while(result > 0){
                    answer[i--] = result % 10;
                    result /= 10;
                }
                // 16 is define in gpio.h replace Number_Dot as Number_Dash
                answer[i] = 16;
            }
        }

        // Show numbers on 7 segments LEDs.
        for(remain = 0 ; remain < 10000 ; remain ++){
            for(j = 0 ; j < 4 ; j++){
                GPIO_PTD_GPIO = 0x0000;
                GPIO_PTA_GPIO = led_index[led - j];
                if(input[j] != 0xFF)
                    GPIO_PTD_GPIO = index_7LED_NUM[input[j]];
                else
                    GPIO_PTD_GPIO = 0x0000;
            }
            if(sign > 0){
                for(j = 0 ; j < 4 ; j++){
                    GPIO_PTD_GPIO = 0x0000;
                    GPIO_PTA_GPIO = led_index[led - j - 4];
                    if(answer[j] != 0xFF)
                        GPIO_PTD_GPIO = index_7LED_NUM[answer[j]];
                    else
                        GPIO_PTD_GPIO = index_7LED_NUM[0];
                }
            }
        }

    }
    return 0;
}

