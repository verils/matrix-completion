
void normal_equation(double *theta, const int *x, const int *y, int size);

void batch_gradient_descent(double *theta, double alpha, int steps, const int *x, const int *y, int size);

void stochastic_gradient_descent(double *theta, double alpha, int steps, const int *x, const int *y, int size);

void
mini_batch_gradient_descent(double *theta, double alpha, int steps, int batch_size, const int *x, const int *y,
                            int size);
