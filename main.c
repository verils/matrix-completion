#include <stdio.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define MAX_DATA_SIZE 560000
#define MAX_DATA_SET_SIZE 4000
//#define MAX_CITY_DATA_SIZE 1500
#define MAX_CITY_DATA_SET_SIZE 32

typedef struct {
    int current;
    clock_t clocks[16];
} Timer;

void timer_record(Timer *timer) {
    timer->clocks[timer->current++] = clock();
}

double timer_milliseconds(Timer *timer, int start, int end) {
    if (start >= end) {
        return -1;
    }
    if (end >= timer->current) {
        return -1;
    }
    return (double) (timer->clocks[end] - timer->clocks[start]) * 1000 / CLOCKS_PER_SEC;
}

void copy_array(const double source[], double target[], int size) {
    for (int i = 0; i < size; ++i) {
        target[i] = source[i];
    }
}

int main() {
    Timer timer = {.current = 0};
    timer_record(&timer);

    static DailyAirData air_data[MAX_DATA_SET_SIZE] = {NULL};
    static DailyAirData first_city_air_data[MAX_CITY_DATA_SET_SIZE] = {NULL};
    int data_size;
    int city_data_size;

    data_size = air_data_read_csv(FILENAME, air_data, MAX_DATA_SET_SIZE);
    printf("Air quality data set size: %d, memory: %luKB\n", data_size, data_size * sizeof(DailyAirData) / 1024);

    timer_record(&timer);

    char *first_city = air_data[0].city;
    int max_city_size = data_size < MAX_CITY_DATA_SET_SIZE ? data_size : MAX_CITY_DATA_SET_SIZE;

    city_data_size = air_data_copy_for_city(air_data, first_city_air_data, first_city, max_city_size);
    printf("Air quality data set size in city '%s': %d\n", first_city, city_data_size);

    timer_record(&timer);

    air_data_sort(first_city_air_data, city_data_size);

    timer_record(&timer);

    double features[city_data_size], results[city_data_size], sum_feature = 0, sum_result = 0;
    for (int i = 0; i < city_data_size; ++i) {
        double feature = (double) first_city_air_data[i].day_of_unix_epoch / 5000;
        double result = (double) first_city_air_data[i].aqi / 30;
        features[i] = feature;
        results[i] = result;
        sum_feature += feature;
        sum_result += result;
    }

    double estimate_scale = sum_result / sum_feature;

    DataSet data_set = {
            .features=features,
            .results=results,
            .size=city_data_size
    };

    timer_record(&timer);

    double initial_theta[2] = {0, 0};

    double ne_theta[2];
    copy_array(initial_theta, ne_theta, 2);
    normal_equation(&data_set, ne_theta);

    timer_record(&timer);

    double bgd_theta[2], bgd_alpha = .01;
    copy_array(initial_theta, bgd_theta, 2);
    const int bgd_steps = 100000;
    batch_gradient_descent(&data_set, bgd_theta, bgd_alpha, bgd_steps);

    timer_record(&timer);

    double sgd_theta[2], sgd_alpha = .001;
    copy_array(initial_theta, sgd_theta, 2);
    const int sgd_steps = 250000;
    stochastic_gradient_descent(&data_set, sgd_theta, sgd_alpha, sgd_steps);

    timer_record(&timer);

    double mbgd_theta[2], mbgd_alpha = .001;
    copy_array(initial_theta, mbgd_theta, 2);
    const int mbgd_steps = 250000, mbgd_batch_size = 8;
    mini_batch_gradient_descent(&data_set, mbgd_theta, mbgd_alpha, mbgd_steps, mbgd_batch_size);

    timer_record(&timer);

    printf("Normal equation: theta[0]=%f, theta[1]=%f, squares_error=%f\n",
           ne_theta[0], ne_theta[1], squares_error(ne_theta, &data_set));
    printf("Batch gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f\n",
           bgd_theta[0], bgd_theta[1], squares_error(bgd_theta, &data_set));
    printf("Stochastic gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f\n",
           sgd_theta[0], sgd_theta[1], squares_error(sgd_theta, &data_set));
    printf("Mini batch gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f\n",
           mbgd_theta[0], mbgd_theta[1], squares_error(mbgd_theta, &data_set));

    printf("Air data read time usage: %.2fms\n", timer_milliseconds(&timer, 0, 1));
    printf("Air data sort time usage: %.2fms\n", timer_milliseconds(&timer, 2, 3));
    printf("Air data normal equation fitting usage: %.2fms\n", timer_milliseconds(&timer, 4, 5));
    printf("Air data batch gradient descent fitting usage: %.2fms\n", timer_milliseconds(&timer, 5, 6));
    printf("Air data stochastic gradient descent fitting usage: %.2fms\n", timer_milliseconds(&timer, 6, 7));
    printf("Air data mini batch gradient descent fitting usage: %.2fms\n", timer_milliseconds(&timer, 7, 8));
    printf("total usage: %.2fms\n", timer_milliseconds(&timer, 0, 8));

    return 0;
}
