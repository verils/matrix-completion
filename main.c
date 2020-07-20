#include <stdio.h>
#include "air_data.h"
#include "regression.h"

#define DATA_SIZE 16

int main() {
    char *filename = "air.csv";

    int data_size = DATA_SIZE;
    DailyAirData data[data_size];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return 1;
    }

    read_air_data_csv(fp, data, data_size);

    fclose(fp);

    printf("AQI: \n");
    for (int i = 0; i < data_size; ++i) {
        printf("%s, %d\n", data[i].date, data[i].aqi);
    }
    printf("\n");

    int aqi_data[data_size];
    for (int i = 0; i < data_size; ++i) {
        aqi_data[i] = data[i].aqi;
    }

    float a, b;
    least_squares_fitting(aqi_data, data_size, &a, &b);

    printf("a=%f, b=%f", a, b);

    return 0;
}