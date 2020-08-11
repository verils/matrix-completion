#include <stdio.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define MAX_DATA_SIZE 560000
#define MAX_DATA_SET_SIZE 4000
//#define MAX_CITY_DATA_SIZE 1500
#define MAX_CITY_DATA_SET_SIZE 256

typedef struct {
    int current;
    clock_t clocks[16];
} Timer;

int timer_record(Timer *timer) {
    timer->clocks[timer->current] = clock();
    return timer->current++;
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
    int timer_start = timer_record(&timer);

    static DailyAirData air_data[MAX_DATA_SET_SIZE] = {NULL};
    static DailyAirData first_city_air_data[MAX_CITY_DATA_SET_SIZE] = {NULL};
    int data_size;
    int city_data_size;

    data_size = air_data_read_csv(FILENAME, air_data, MAX_DATA_SET_SIZE);
    printf("Air quality data set size: %d, memory: %luKB\n", data_size, data_size * sizeof(DailyAirData) / 1024);

    char *first_city = air_data[0].city;
    int max_city_size = data_size < MAX_CITY_DATA_SET_SIZE ? data_size : MAX_CITY_DATA_SET_SIZE;

    city_data_size = air_data_copy_for_city(air_data, first_city_air_data, first_city, max_city_size);
    printf("Air quality data set size in city '%s': %d\n", first_city, city_data_size);

    int timer_file_read = timer_record(&timer);

    air_data_sort(first_city_air_data, city_data_size);

    int timer_data_sorted = timer_record(&timer);

    double features[city_data_size], results[city_data_size];
    int days_of_unix_epoch_scale = 5000;
    int aqi_scale = 30;
    for (int i = 0; i < city_data_size; ++i) {
        features[i] = (double) first_city_air_data[i].day_of_unix_epoch / days_of_unix_epoch_scale;
        results[i] = (double) first_city_air_data[i].aqi / aqi_scale;
    }

    DataSet data_set = {
            .features=features,
            .results=results,
            .size=city_data_size
    };

    int timer_data_prepared = timer_record(&timer);

    double initial_theta[2] = {0, 0};

    double ne_theta[2];
    copy_array(initial_theta, ne_theta, 2);
    train_normal_equation(ne_theta, &data_set);
    printf("Normal equation: theta[0]=%f, theta[1]=%f, squares_error=%f\n",
           ne_theta[0], ne_theta[1], squares_error(ne_theta, &data_set));

    int timer_ne = timer_record(&timer);

    double bgd_theta[2];
    copy_array(initial_theta, bgd_theta, 2);
    BatchGradientDescent batch_gradient_descent = {
            .theta = bgd_theta,
            .dimension=2,
            .alpha = .13333333,
            .expected_error=.02405
    };
    train_batch_gradient_descent(&batch_gradient_descent, &data_set);
    printf("Batch gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f, steps=%d\n",
           bgd_theta[0], bgd_theta[1], squares_error(bgd_theta, &data_set), batch_gradient_descent.steps);

    int timer_bgd = timer_record(&timer);

    double sgd_theta[2];
    copy_array(initial_theta, sgd_theta, 2);
    StochasticGradientDescent stochastic_gradient_descent = {
            .theta = sgd_theta,
            .dimension=2,
            .alpha = .13333333,
            .expected_error=.02405
    };
    train_stochastic_gradient_descent(&stochastic_gradient_descent, &data_set);
    printf("Stochastic gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f, steps=%d\n",
           sgd_theta[0], sgd_theta[1], squares_error(sgd_theta, &data_set), stochastic_gradient_descent.steps);

    int timer_sgd = timer_record(&timer);

    double mbgd_theta[2];
    copy_array(initial_theta, mbgd_theta, 2);
    MiniBatchGradientDescent mini_batch_gradient_descent = {
            .theta = mbgd_theta,
            .dimension=2,
            .alpha = .13333333,
            .expected_error=.02405,
            .batch_size=8
    };
    train_mini_batch_gradient_descent(&mini_batch_gradient_descent, &data_set);
    printf("Mini batch gradient descent: theta[0]=%f, theta[1]=%f, squares_error=%f, steps=%d\n",
           mbgd_theta[0], mbgd_theta[1], squares_error(mbgd_theta, &data_set), mini_batch_gradient_descent.steps);

    int timer_mbgd = timer_record(&timer);

    printf("Air data read time usage: %.3fms\n",
           timer_milliseconds(&timer, timer_start, timer_file_read));
    printf("Air data sort time usage: %.3fms\n",
           timer_milliseconds(&timer, timer_file_read, timer_data_sorted));
    printf("Air data normal equation fitting usage: %.3fms\n",
           timer_milliseconds(&timer, timer_data_prepared, timer_ne));
    printf("Air data batch gradient descent fitting usage: %.3fms\n",
           timer_milliseconds(&timer, timer_ne, timer_bgd));
    printf("Air data stochastic gradient descent fitting usage: %.3fms\n",
           timer_milliseconds(&timer, timer_bgd, timer_sgd));
    printf("Air data mini batch gradient descent fitting usage: %.3fms\n",
           timer_milliseconds(&timer, timer_sgd, timer_mbgd));
    printf("total usage: %.2fms\n",
           timer_milliseconds(&timer, timer_start, timer_mbgd));

    return 0;
}
