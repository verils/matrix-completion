#include <stdlib.h>
#include <time.h>

double hypothesis(const double theta[2], const double x) {
    return theta[0] + theta[1] * x;
}

double squared_error(const double theta[2], const int x[], const int y[], const int size) {
    double sum = 0;
    for (int i = 0; i < size; ++i) {
        double error = hypothesis(theta, x[i]) - y[i];
        sum += error * error;
    }
    return sum / 2 / size;
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

void normal_equation(double *theta, const int *x, const int *y, int size) {
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    for (int i = 0; i < size; ++i) {
        sum_x += (double) x[i];
        sum_y += (double) y[i];
        sum_xx += (double) x[i] * (double) x[i];
        sum_xy += (double) x[i] * (double) y[i];
    }
    double size_d = (double) size;
    avg_x = sum_x / size_d;
    avg_y = sum_y / size_d;
    avg_xx = sum_xx / size_d;
    avg_xy = sum_xy / size_d;
    theta[1] = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    theta[0] = (sum_y - theta[1] * sum_x) / size_d;
}

void batch_gradient_descent(double *theta, double alpha, int steps, const int *x, const int *y, int size) {
    for (int step = 0; step < steps; ++step) {
        double sum_0 = 0, sum_1 = 0, theta_0, theta_1;
        for (int i = 0; i < size; ++i) {
            sum_0 += theta[0] + theta[1] * x[i] - y[i];
            sum_1 += (theta[0] + theta[1] * x[i] - y[i]) * x[i];
        }
        double avg_0 = sum_0 / size, avg_1 = sum_1 / size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

void stochastic_gradient_descent(double theta[2], double alpha, int steps, const int *x, const int *y, int size) {
    srand(time(NULL));
    for (int step = 0; step < steps; ++step) {
        double theta_0, theta_1;
        int index = rand() % size;
        theta_0 = theta[0] - alpha * (theta[0] + theta[1] * x[index] - y[index]);
        theta_1 = theta[1] - alpha * ((theta[0] + theta[1] * x[index] - y[index]) * x[index]);
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

void
mini_batch_gradient_descent(double *theta, double alpha, int steps, int batch_size, const int *x, const int *y,
                            int size) {
    srand(time(NULL));
    for (int step = 0; step < steps; ++step) {
        double sum_0 = 0, sum_1 = 0, theta_0, theta_1;
        for (int i = 0; i < batch_size; ++i) {
            int index = rand() % size;
            sum_0 += theta[0] + theta[1] * x[index] - y[index];
            sum_1 += (theta[0] + theta[1] * x[index] - y[index]) * x[index];
        }
        double avg_0 = sum_0 / size, avg_1 = sum_1 / size;
        theta_0 = theta[0] - alpha * avg_0;
        theta_1 = theta[1] - alpha * avg_1;
        theta[0] = theta_0;
        theta[1] = theta_1;
    }
}

