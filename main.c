#include <stdio.h>
#include <stdlib.h>
#include "air_data.h"
#include "regression.h"

#define FILENAME "air.csv"

#define DATA_SIZE 50
//#define DATA_SIZE 500000

int main() {
    char *filename = FILENAME;

    static DailyAirData data[DATA_SIZE];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return 1;
    }

    read_air_data_csv(fp, data, DATA_SIZE);

    fclose(fp);

    qsort(data, DATA_SIZE, sizeof(DailyAirData), compare_air_date);

    printf("Air Quality Data: \n");
    print_air_data(data, DATA_SIZE);

    int aqi_data[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; ++i) {
        aqi_data[i] = data[i].aqi;
    }

    float a, b;
    least_squares_fitting(aqi_data, DATA_SIZE, &a, &b);

    printf("a=%f, b=%f", a, b);

    return 0;
}
