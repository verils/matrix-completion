#include <stdlib.h>
#include <time.h>
#include "linear_regression.h"

double hypothesis(const double theta[2], const double x) {
    return theta[0] + theta[1] * x;
}

double squared_error(const double theta[2], const DataSet *data_set) {
    double sum = 0;
    for (int i = 0; i < data_set->size; ++i) {
        double error = hypothesis(theta, data_set->x[i]) - data_set->y[i];
        sum += error * error;
    }
    return sum / 2 / data_set->size;
}

double squared_error_derivative_theta_0(const double theta[2], const double x, const double y) {
    return theta[0] + theta[1] * x - y;
}

double squared_error_derivative_theta_1(const double theta[2], const double x, const double y) {
    return (theta[0] + theta[1] * x - y) * x;
}

void simulated_annealing() {

}

//L(a,b)=(1/2m)sum(1,m)(a+bx-y)^2
//L(a,b)=(1/2m)sum(1,m)(a^2+abx-ay+abx+(b^2)(x^2)-bxy-ay-bxy+y^2)
//L(a,b)=(1/2m)sum(1,m)(a^2+2abx-2ay+(b^2)(x^2)-2bxy+y^2)
//
//d/da(L(a,b))=(1/2m)sum(1,m)(2a+2bx-2y)
//d/da(L(a,b))=(1/2m)sum(1,m) 2(a+bx-y)
//d/da(L(a,b))=(1/2m)2 sum(1,m)(a+bx-y)
//d/da(L(a,b))=(1/m) sum(1,m)(a+bx-y)
//
//d/db(L(a,b))=(1/2m)sum(1,m)(2ax+2b(x^2)-2xy)
//d/db(L(a,b))=(1/2m)sum(1,m) ((2a+2bx-2y)x)
//d/db(L(a,b))=(1/2m)sum(1,m) 2((a+bx-y)x)
//d/db(L(a,b))=(1/2m)2 sum(1,m)((a+bx-y)x)
//d/db(L(a,b))=(1/m) sum(1,m)((a+bx-y)x)

void normal_equation(double *theta, const DataSet *data_set) {
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    for (int i = 0; i < data_set->size; ++i) {
        double x_d = (double) data_set->x[i];
        double y_d = (double) data_set->y[i];
        sum_x += x_d;
        sum_y += y_d;
        sum_xx += x_d * x_d;
        sum_xy += x_d * y_d;
    }
    double size_d = (double) data_set->size;
    avg_x = sum_x / size_d;
    avg_y = sum_y / size_d;
    avg_xx = sum_xx / size_d;
    avg_xy = sum_xy / size_d;
    theta[1] = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    theta[0] = (sum_y - theta[1] * sum_x) / size_d;
}

void batch_gradient_descent(double *theta, double alpha, int steps, const DataSet *data_set) {
    for (int step = 0; step < steps; ++step) {
        double sum_d_theta_0 = 0, sum_d_theta_1 = 0, theta_0, theta_1;
        for (int i = 0; i < data_set->size; ++i) {
            sum_d_theta_0 += squared_error_derivative_theta_0(theta, data_set->x[i], data_set->y[i]);
            sum_d_theta_1 += squared_error_derivative_theta_1(theta, data_set->x[i], data_set->y[i]);
        }
        double avg_0 = sum_d_theta_0 / data_set->size, avg_1 = sum_d_theta_1 / data_set->size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

void stochastic_gradient_descent(double theta[2], double alpha, int steps, const DataSet *data_set) {
    srand(time(NULL));
    for (int step = 0; step < steps; ++step) {
        int index = rand() % data_set->size;
        double d_theta_0 = squared_error_derivative_theta_0(theta, data_set->x[index], data_set->y[index]);
        double d_theta_1 = squared_error_derivative_theta_1(theta, data_set->x[index], data_set->y[index]);
        double theta_0 = theta[0] - alpha * d_theta_0;
        double theta_1 = theta[1] - alpha * d_theta_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

void
mini_batch_gradient_descent(double *theta, double alpha, int steps, int batch_size, const DataSet *data_set) {
    srand(time(NULL));
    for (int step = 0; step < steps; ++step) {
        double sum_0 = 0, sum_1 = 0, theta_0, theta_1;
        for (int i = 0; i < batch_size; ++i) {
            int index = rand() % data_set->size;
            sum_0 += squared_error_derivative_theta_0(theta, data_set->x[index], data_set->y[index]);
            sum_1 += squared_error_derivative_theta_1(theta, data_set->x[index], data_set->y[index]);
        }
        double avg_0 = sum_0 / batch_size, avg_1 = sum_1 / batch_size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

