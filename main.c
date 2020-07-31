#include <stdio.h>
#include <time.h>
#include "air_data.h"
#include "linear_regression.h"

#define FILENAME "air.csv"

//#define MAX_DATA_SIZE 560000
#define MAX_DATA_SIZE 1500
//#define MAX_CITY_DATA_SIZE 1500
#define MAX_CITY_DATA_SIZE 32

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
    static DailyAirData city_data[MAX_CITY_DATA_SIZE] = {NULL};
    int data_size;
    int city_data_size;

    data_size = air_data_read_csv(FILENAME, data, MAX_DATA_SIZE);
    printf("Air quality data size: %d\n", data_size);

    timer_record(&timer);

    char *first_city = data[0].city;
    int max_city_size = data_size < MAX_CITY_DATA_SIZE ? data_size : MAX_CITY_DATA_SIZE;

    city_data_size = air_data_copy_for_city(data, city_data, first_city, max_city_size);
    printf("Air quality data size in city '%s': %d\n", first_city, city_data_size);

    timer_record(&timer);

    air_data_sort(city_data, city_data_size);

    timer_record(&timer);

    int days_of_unix_epoch[city_data_size];
    int city_aqi_data[city_data_size];

    for (int i = 0; i < city_data_size; ++i) {
        days_of_unix_epoch[i] = city_data[i].days_of_unix_epoch;
        city_aqi_data[i] = city_data[i].aqi;
    }

    timer_record(&timer);

    double ne_theta[2] = {0};
    normal_equation(ne_theta, days_of_unix_epoch, city_aqi_data, city_data_size);

    timer_record(&timer);

    double bgd_theta[2] = {0}, bgd_alpha = 0.003;
    const int bgd_steps = 20000;
    batch_gradient_descent(bgd_theta, bgd_alpha, bgd_steps, days_of_unix_epoch, city_aqi_data, city_data_size);

    timer_record(&timer);

    double sgd_theta[2] = {0}, sgd_alpha = 0.001;
    const int sgd_steps = 250000;
    stochastic_gradient_descent(sgd_theta, sgd_alpha, sgd_steps, days_of_unix_epoch, city_aqi_data, city_data_size);

    timer_record(&timer);

    double mbgd_theta[2] = {0}, mbgd_alpha = 0.003;
    const int mbgd_steps = 20000, mbgd_batch_size = 4;
    mini_batch_gradient_descent(mbgd_theta, mbgd_alpha, mbgd_steps, mbgd_batch_size, days_of_unix_epoch, city_aqi_data,
                                city_data_size);

    timer_record(&timer);

    printf("Normal equation: theta[0]=%f, theta[1]=%f\n", ne_theta[0], ne_theta[1]);
    printf("Batch gradient descent: theta[0]=%f, theta[1]=%f\n", bgd_theta[0], bgd_theta[1]);
    printf("Stochastic gradient descent: theta[0]=%f, theta[1]=%f\n", sgd_theta[0], sgd_theta[1]);
    printf("Mini batch gradient descent: theta[0]=%f, theta[1]=%f\n", mbgd_theta[0], mbgd_theta[1]);

    printf("Air data read time usage: %.2fms\n", timer_milliseconds(&timer, 0, 1));
    printf("Air data sort time usage: %.2fms\n", timer_milliseconds(&timer, 2, 3));
    printf("Air data normal equation fitting usage: %.2fms\n", timer_milliseconds(&timer, 4, 5));
    printf("Air data batch gradient descent usage: %.2fms\n", timer_milliseconds(&timer, 5, 6));
    printf("Air data stochastic gradient descent usage: %.2fms\n", timer_milliseconds(&timer, 6, 7));
    printf("Air data mini batch gradient descent usage: %.2fms\n", timer_milliseconds(&timer, 7, 8));
    printf("total usage: %.2fms\n", timer_milliseconds(&timer, 0, 8));

    return 0;
}
