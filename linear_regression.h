
void fit_normal_equation(const int *x, const int *y, int size, double *theta);

void fit_batch_gradient_descent(const int *x, const int *y, int size, const double *alpha, double theta[2], int steps,
                                double theta_history[steps][2]);