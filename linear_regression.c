#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linear_regression.h"

double hypothesis(const double *theta, const double x) {
    return theta[0] + theta[1] * x;
}

double squares_error(const double *theta, const DataSet *data_set) {
    double *x = data_set->features;
    double *y = data_set->results;
    int size = data_set->size;
    double sum = 0;
    for (int i = 0; i < size; ++i) {
        double error = hypothesis(theta, x[i]) - y[i];
        sum += error * error;
    }
    return sum / 2 / size;
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

void normal_equation(const DataSet *data_set, double *theta) {
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;
    for (int i = 0; i < size; ++i) {
        double x_d = features[i];
        double y_d = results[i];
        sum_x += x_d;
        sum_y += y_d;
        sum_xx += x_d * x_d;
        sum_xy += x_d * y_d;
    }
    double size_d = (double) size;
    avg_x = sum_x / size_d;
    avg_y = sum_y / size_d;
    avg_xx = sum_xx / size_d;
    avg_xy = sum_xy / size_d;
    theta[1] = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    theta[0] = (sum_y - theta[1] * sum_x) / size_d;
}

void batch_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps) {
    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;
    for (int step = 0; step < steps; ++step) {
        double sum_d_theta_0 = 0, sum_d_theta_1 = 0, theta_0, theta_1;
        for (int i = 0; i < size; ++i) {
            sum_d_theta_0 += squared_error_derivative_theta_0(theta, features[i], results[i]);
            sum_d_theta_1 += squared_error_derivative_theta_1(theta, features[i], results[i]);
        }
        double avg_0 = sum_d_theta_0 / size, avg_1 = sum_d_theta_1 / size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

void stochastic_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps) {
    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;
    srand(time(NULL));
    for (int step = 0; step < steps; ++step) {
        int index = rand() % size;
        double d_theta_0 = squared_error_derivative_theta_0(theta, features[index], results[index]);
        double d_theta_1 = squared_error_derivative_theta_1(theta, features[index], results[index]);
        double theta_0 = theta[0] - alpha * d_theta_0;
        double theta_1 = theta[1] - alpha * d_theta_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
//        printf("Stochastic gradient: theta_0=%f, theta_1=%f, squares_error=%f\n", theta[0], theta[1],
//               squares_error(theta, data_set));
    }
}

void mini_batch_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps, int batch_size) {
    double *features = data_set->features;
    double *results = data_set->results;
    srand(time(NULL));
    int size = data_set->size;
    for (int step = 0; step < steps; ++step) {
        double sum_0 = 0, sum_1 = 0, theta_0, theta_1;
        for (int i = 0; i < batch_size; ++i) {
            int index = rand() % size;
            sum_0 += squared_error_derivative_theta_0(theta, features[index], results[index]);
            sum_1 += squared_error_derivative_theta_1(theta, features[index], results[index]);
        }
        double avg_0 = sum_0 / batch_size, avg_1 = sum_1 / batch_size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

