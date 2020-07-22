#include <stdio.h>
#include <stdlib.h>
#include "air_data.h"
#include "regression.h"

#define FILENAME "air.csv"

//#define DATA_SIZE 50
#define CAPACITY 560000

int main() {
    char *filename = FILENAME;
    static DailyAirData data[CAPACITY];
    int size;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return 1;
    }

    read_air_data_csv(fp, data, &size, CAPACITY);

    fclose(fp);

    printf("Air quality data size: %d\n", size);

    qsort(data, size, sizeof(DailyAirData), compare_air_date);

    int aqi_data[size];
    for (int i = 0; i < size; ++i) {
        aqi_data[i] = data[i].aqi;
    }

    float a, b;
    least_squares_fitting(aqi_data, size, &a, &b);

    printf("a=%f, b=%f", a, b);

    return 0;
}
