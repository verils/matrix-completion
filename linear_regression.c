#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

double derivative(const double *theta, const double *x, int k, double y) {
    return (theta[0] * x[0] + theta[1] * x[1] - y) * x[k];
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

void normal_equation_train(double *theta, const DataSet *data_set) {
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

double batch_gradient_descent_optimize(double initial_learning_rate, int iterations, int mode) {
    if (mode == OPTIMIZATION_FIXED) {
        return initial_learning_rate;
    } else if (mode == OPTIMIZATION_EXPONENTIAL) {
        int exponent = iterations / 100;
        return (double) initial_learning_rate * pow(.96, (double) exponent);
    }
    return initial_learning_rate;
}

void batch_gradient_descent_train(BatchGradientDescent *bgd, const DataSet *data_set) {
    double *theta = bgd->theta;

    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;

    while (squares_error(theta, data_set) > bgd->expected_error && bgd->iterations < bgd->max_iterations) {
        double gradient[2] = {0};
        for (int i = 0; i < size; ++i) {
            double x[2] = {1, features[i]};
            gradient[0] += derivative(theta, x, 0, results[i]) / size;
            gradient[1] += derivative(theta, x, 1, results[i]) / size;
        }

        double learning_rate = batch_gradient_descent_optimize(
                bgd->learning_rate, bgd->iterations, bgd->optimization_mode);
        theta[0] = theta[0] - learning_rate * gradient[0];
        theta[1] = theta[1] - learning_rate * gradient[1];
        bgd->iterations++;
    }
}

void stochastic_gradient_descent_train(StochasticGradientDescent *sgd, const DataSet *data_set) {
    MiniBatchGradientDescent mbgd = {
            .theta=sgd->theta,
            .dimension=sgd->dimension,
            .learning_rate=sgd->learning_rate,
            .expected_error=sgd->expected_error,
            .max_iterations=sgd->max_iterations,
            .batch_size=1
    };
    mini_batch_gradient_descent_train(&mbgd, data_set);
    sgd->iterations = mbgd.iterations;
}

void mini_batch_gradient_descent_train(MiniBatchGradientDescent *mbgd, const DataSet *data_set) {
    srand(time(NULL));

    double *theta = mbgd->theta;
    int batch_size = mbgd->batch_size;

    double *features = data_set->features;
    double *results = data_set->results;
    int size = data_set->size;

    while (squares_error(theta, data_set) > mbgd->expected_error && mbgd->iterations < mbgd->max_iterations) {
        double gradient[2] = {0};
        for (int i = 0; i < batch_size; ++i) {
            int index = rand() % size;
            double x[2] = {1, features[index]};
            gradient[0] += derivative(theta, x, 0, results[index]) / batch_size;
            gradient[1] += derivative(theta, x, 1, results[index]) / batch_size;
        }
        theta[0] = theta[0] - mbgd->learning_rate * gradient[0];
        theta[1] = theta[1] - mbgd->learning_rate * gradient[1];
        mbgd->iterations++;
    }
}

