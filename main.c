#include <stdio.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define DATA_SIZE 50
#define MAX_DATA_SIZE 560000
//#define MAX_CITY_DATA_SIZE 1500
#define MAX_CITY_DATA_SIZE 1200

int main() {
    time_t beginning_clock = clock();

    static DailyAirData data[MAX_DATA_SIZE] = {NULL};
    int size;

    air_data_read_csv(data, MAX_DATA_SIZE, &size, FILENAME);
    printf("Air quality data size: %d\n", size);

    char *first_city = data[0].city;
    static DailyAirData city_data[MAX_CITY_DATA_SIZE];
    int max_city_size = size < MAX_CITY_DATA_SIZE ? size : MAX_CITY_DATA_SIZE;
    int city_size = 0;
    air_data_filter_by_city(data, city_data, first_city, max_city_size, &city_size);
    printf("Air quality data size in city '%s': %d\n", first_city, city_size);

    clock_t read_file_clock = clock();
    double read_file_duration = difftime(read_file_clock, beginning_clock);

    air_data_sort(city_data, city_size);

    clock_t sort_data_clock = clock();
    double sort_data_duration = difftime(sort_data_clock, read_file_clock);

    int days_of_unix_epoch[city_size];
    int city_aqi_data[city_size];
    for (int i = 0; i < city_size; ++i) {
        days_of_unix_epoch[i] = city_data[i].days_of_unix_epoch;
        city_aqi_data[i] = city_data[i].aqi;
    }

    float ls_theta_0, ls_theta_1;
    fit_normal_equation(days_of_unix_epoch, city_aqi_data, city_size, &ls_theta_0, &ls_theta_1);
    printf("Normal equation: theta0=%f, theta1=%f\n", ls_theta_0, ls_theta_1);

    float alpha, sgd_theta_0, sgd_theta_1;
    fit_batch_gradient_descent(city_aqi_data, city_size, &alpha, &sgd_theta_0, &sgd_theta_1);

    clock_t fitting_clock = clock();
    double fitting_duration = difftime(fitting_clock, sort_data_clock);
    double total_duration = difftime(fitting_clock, beginning_clock);

    printf("read file usage: %.2fms\n", read_file_duration * 1000 / CLOCKS_PER_SEC);
    printf("sort data usage: %.2fms\n", sort_data_duration * 1000 / CLOCKS_PER_SEC);
    printf("fitting usage: %.2fms\n", fitting_duration * 1000 / CLOCKS_PER_SEC);
    printf("total usage: %.2fms\n", total_duration * 1000 / CLOCKS_PER_SEC);

    return 0;
}
