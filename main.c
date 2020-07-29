#include <stdio.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define DATA_SIZE 50
#define MAX_DATA_SIZE 560000
//#define MAX_CITY_DATA_SIZE 1500
#define MAX_CITY_DATA_SIZE 12

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

int main() {
    Timer timer = {.current = 0};
    timer_record(&timer);

    static DailyAirData data[MAX_DATA_SIZE] = {NULL};
    int size;
    air_data_read_csv(data, MAX_DATA_SIZE, &size, FILENAME);
    printf("Air quality data size: %d\n", size);
    timer_record(&timer);

    char *first_city = data[0].city;
    static DailyAirData city_data[MAX_CITY_DATA_SIZE];
    int max_city_size = size < MAX_CITY_DATA_SIZE ? size : MAX_CITY_DATA_SIZE;
    int city_size = 0;
    air_data_filter_by_city(data, city_data, first_city, max_city_size, &city_size);
    printf("Air quality data size in city '%s': %d\n", first_city, city_size);
    timer_record(&timer);

    air_data_sort(city_data, city_size);
    timer_record(&timer);

    int days_of_unix_epoch[city_size];
    int city_aqi_data[city_size];
    for (int i = 0; i < city_size; ++i) {
        days_of_unix_epoch[i] = city_data[i].days_of_unix_epoch;
        city_aqi_data[i] = city_data[i].aqi;
    }
    timer_record(&timer);

    double ne_theta[2] = {0};
    fit_normal_equation(days_of_unix_epoch, city_aqi_data, city_size, ne_theta);
    timer_record(&timer);

    const int steps = 100;
    double alpha = 0.000000003, bgd_theta[2] = {50, 0};
    double bgd_theta_history[steps][2];
    for (int i = 0; i < steps; ++i) {
        bgd_theta_history[i][0] = 0;
    }
    fit_batch_gradient_descent(days_of_unix_epoch, city_aqi_data, city_size, &alpha, bgd_theta, steps,
                               bgd_theta_history);
    timer_record(&timer);

    printf("Normal equation: theta[0]=%f, theta[1]=%f\n", ne_theta[0], ne_theta[1]);
    for (int i = 0; i < steps; ++i) {
        printf("Batch gradient descent: step=%d, theta[0]=%f, theta[1]=%f\n", i,
               bgd_theta_history[i][0], bgd_theta_history[i][1]);
    }

    printf("Air data read time usage: %.2fms\n", timer_milliseconds(&timer, 0, 1));
    printf("Air data sort time usage: %.2fms\n", timer_milliseconds(&timer, 2, 3));
    printf("Air data normal equation fitting usage: %.2fms\n", timer_milliseconds(&timer, 4, 5));
    printf("Air data batch gradient descent usage: %.2fms\n", timer_milliseconds(&timer, 5, 6));
    printf("total usage: %.2fms\n", timer_milliseconds(&timer, 0, 6));

    return 0;
}
