#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define DATA_SIZE 50
#define CAPACITY 560000

int main() {
    time_t beginning_clock = clock() / CLOCKS_PER_SEC;

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

    clock_t read_file_clock = clock() / CLOCKS_PER_SEC;
    double read_file_duration = difftime(read_file_clock, beginning_clock);

    printf("Air quality data size: %d\n", size);

    qsort(data, size, sizeof(DailyAirData), compare_air_date);

    clock_t sort_data_clock = clock() / CLOCKS_PER_SEC;
    double sort_data_duration = difftime(sort_data_clock, read_file_clock);

    int aqi_data[size];
    for (int i = 0; i < size; ++i) {
        aqi_data[i] = data[i].aqi;
    }

    float theta_0, theta_1;
    least_squares_fitting(aqi_data, size, &theta_0, &theta_1);

    gradient_descent_fitting(aqi_data, size, &theta_0, &theta_1);

    clock_t fitting_clock = clock() / CLOCKS_PER_SEC;
    double fitting_duration = difftime(fitting_clock, sort_data_clock);
    double total_duration = difftime(fitting_clock, beginning_clock);

    printf("theta0=%f, theta1=%f\n", theta_0, theta_1);

    printf("read file usage: %fs\n", read_file_duration);
    printf("sort data usage: %fs\n", sort_data_duration);
    printf("fitting usage: %fs\n", fitting_duration);
    printf("total usage: %fs\n", total_duration);

    return 0;
}
