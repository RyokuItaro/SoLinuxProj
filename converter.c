#include "headers/converter.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

int parseTime(char *time){
    int seconds = 0;
    int decimalPlaces = strlen(time);
    for(int i = 0; i < strlen(time); i++){
        seconds += (time[i]-48) * pow(10, decimalPlaces-1);
        printf("sec %d \n", seconds);
        printf("places %d \n", decimalPlaces-1);
        decimalPlaces--;
    }
    return seconds;
}