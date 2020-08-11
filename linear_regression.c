#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linear_regression.h"

double hypothesis(const double *theta, const double x) {
    return theta[0] + theta[1] * x;
}

double squares_error(const double *theta, const DataSet *data_set) {
    double sum = 0;
    for (int i = 0; i < data_set->size; ++i) {
        double error = hypothesis(theta, data_set->features[i]) - data_set->results[i];
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

void train_normal_equation(double *theta, const DataSet *data_set) {
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

void train_batch_gradient_descent(BatchGradientDescent *bgd, const DataSet *data_set) {
    double *theta = bgd->theta;
    double *features = data_set->features;
    double *results = data_set->results;
    while (squares_error(theta, data_set) > bgd->expected_error) {
        double sum_d_theta_0 = 0, sum_d_theta_1 = 0, theta_0, theta_1;
        for (int i = 0; i < data_set->size; ++i) {
            sum_d_theta_0 += squared_error_derivative_theta_0(theta, features[i], results[i]);
            sum_d_theta_1 += squared_error_derivative_theta_1(theta, features[i], results[i]);
        }
        double avg_0 = sum_d_theta_0 / data_set->size, avg_1 = sum_d_theta_1 / data_set->size;
        theta_0 = theta[0] - bgd->alpha * avg_0;
        theta_1 = theta[1] - bgd->alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
        bgd->steps++;
    }
}

void train_stochastic_gradient_descent(StochasticGradientDescent *sgd, const DataSet *data_set) {
    MiniBatchGradientDescent mbgd = {
            .theta = sgd->theta,
            .dimension=sgd->dimension,
            .alpha = sgd->alpha,
            .expected_error=sgd->expected_error,
            .batch_size=1
    };
    train_mini_batch_gradient_descent(&mbgd, data_set);
    sgd->steps = mbgd.steps;
}

void train_mini_batch_gradient_descent(MiniBatchGradientDescent *mbgd, const DataSet *data_set) {
    srand(time(NULL));

    double *theta = mbgd->theta;
    int batch_size = mbgd->batch_size;

    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;

    while (squares_error(theta, data_set) > mbgd->expected_error) {
        double sum_0 = 0, sum_1 = 0, theta_0, theta_1;
        for (int i = 0; i < batch_size; ++i) {
            int index = rand() % size;
            sum_0 += squared_error_derivative_theta_0(theta, features[index], results[index]);
            sum_1 += squared_error_derivative_theta_1(theta, features[index], results[index]);
        }
        double avg_0 = sum_0 / batch_size, avg_1 = sum_1 / batch_size;
        theta_0 = theta[0] - mbgd->alpha * avg_0;
        theta_1 = theta[1] - mbgd->alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
        mbgd->steps++;
    }
}

