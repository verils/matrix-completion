#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define DATA_SIZE 50
#define MAX_DATA_SIZE 560000
#define MAX_CITY_DATA_SIZE 1500

int main() {
    time_t beginning_clock = clock();

    static DailyAirData data[MAX_DATA_SIZE] = {NULL};
    int size;

    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Error opening file %s", FILENAME);
        exit(EXIT_FAILURE);
    }
    air_data_read_csv(data, MAX_DATA_SIZE, &size, fp);
    fclose(fp);

    clock_t read_file_clock = clock();
    double read_file_duration = difftime(read_file_clock, beginning_clock);

    printf("Air quality data size: %d\n", size);

    char *first_city = data[0].city;
    static DailyAirData city_data[MAX_CITY_DATA_SIZE];
    int size_of_city = 0;
    air_data_filter_by_city(data, city_data, first_city, size, &size_of_city);

    printf("Air quality data size in city '%s': %d\n", first_city, size_of_city);

    qsort(city_data, size_of_city, sizeof(DailyAirData), air_data_compare);

    clock_t sort_data_clock = clock();
    double sort_data_duration = difftime(sort_data_clock, read_file_clock);

    int city_aqi_data[MAX_CITY_DATA_SIZE];
    for (int i = 0; i < size_of_city; ++i) {
        city_aqi_data[i] = city_data[i].aqi;
    }

//    float ls_theta_0, ls_theta_1;
//    least_squares_fitting(city_aqi_data, size_of_city, &ls_theta_0, &ls_theta_1);
//    printf("least squares: theta0=%f, theta1=%f\n", ls_theta_0, ls_theta_1);

    float alpha, sgd_theta_0, sgd_theta_1;
    bgd_fitting(city_aqi_data, size_of_city, &alpha, &sgd_theta_0, &sgd_theta_1);

    clock_t fitting_clock = clock();
    double fitting_duration = difftime(fitting_clock, sort_data_clock);
    double total_duration = difftime(fitting_clock, beginning_clock);

    printf("read file usage: %.2fms\n", read_file_duration);
    printf("sort data usage: %.2fms\n", sort_data_duration);
    printf("fitting usage: %.2fms\n", fitting_duration);
    printf("total usage: %.2fms\n", total_duration);

    return 0;
}
