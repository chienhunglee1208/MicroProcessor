rtc.h
#ifndef RTC_H_
#define RTC_H_

#define RTC_CONTROL   *((unsigned int *) 0x00200400)
#define RTC_SEC        *((unsigned int *) 0x00200404)
#define RTC_MIN          *((unsigned int *) 0x00200408)
#define RTC_HOUR     *((unsigned int *) 0x0020040C)
#define RTC_DAY           *((unsigned int *) 0x00200410)
#define RTC_WEEK     *((unsigned int *) 0x00200414)
#define RTC_MONTH      *((unsigned int *) 0x00200418)
#define RTC_YEAR      *((unsigned int *) 0x0020041C)

#define To_ASCII 0x30

#endif /* RTC_H_ */


main.c
void InitialRTC(void) {
    RTC_CONTROL = (RTC_CONTROL | 0x80); // Enable RTC
}

void SetupRTCTime(void){
    unsigned int tyear[2] = {1, 7}, tmon = 12, tday[2] = {2, 9}, tweek = 5;
    unsigned int thour[2] = {1, 6}, tmin[2] = {4, 5}, tsec[2] = {0, 0};

    // add code here
    RTC_YEAR = (tyear[0] << 4) + tyear[1];   // BCD encoding
    RTC_MONTH = tmon ;
    RTC_DAY = (tday[0] << 4) + tday[1];
    RTC_WEEK = tweek;

    RTC_HOUR = (thour[0] << 4) + thour[1];
    RTC_MIN = (tmin[0] << 4) + tmin[1];
    RTC_SEC = (tsec[0] << 4) + tsec[1];
}

void DisplayTime(void) {

    char week_array[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    int i;

    // add code here
    unsigned int year = RTC_YEAR, mon = RTC_MONTH, day = RTC_DAY;
    unsigned int hour = RTC_HOUR, min = RTC_MIN, sec = RTC_SEC;
    unsigned int week = RTC_WEEK;

    SetCursor(0, 0x0); //WriteIns(0x8000); //STN LCM,第一列
    // print year/month/date
    char year_array[] = {
            '2', '0', ((year & 0xF0) >> 4) | To_ASCII, (year & 0x0F) | To_ASCII, '/',
            (mon / 10) | To_ASCII, (mon % 10) | To_ASCII, '/',
            ((day & 0xF0) >> 4) | To_ASCII, (day & 0x0F) | To_ASCII
    };
    for(i = 0 ; i < 10 ; i++) WriteData(year_array[i]);

    // print week
    WriteData(' ');
    for(i = 0 ; i < 3 ; i++) WriteData(week_array[week][i]);

    SetCursor(1, 0x0); //WriteIns(0xC000); //STN LCM,第二列
    // print time
    char time_array[] = {
            ((hour & 0xF0) >> 4) + To_ASCII, (hour & 0x0F) + To_ASCII, ':',
            ((min & 0xF0) >> 4) + To_ASCII, (min & 0x0F) + To_ASCII, ':',
            ((sec & 0xF0) >> 4) + To_ASCII, (sec & 0x0F) + To_ASCII
    };
    for(i = 0 ; i < 8 ; i++) WriteData(time_array[i]);
}

