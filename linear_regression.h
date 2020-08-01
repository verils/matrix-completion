
typedef struct {
    double *x;
    double *y;
    int size;
} DataSet;

void normal_equation(double *theta, const DataSet *data_set);

void batch_gradient_descent(double *theta, double alpha, int steps, const DataSet *data_set);

void stochastic_gradient_descent(double *theta, double alpha, int steps, const DataSet *data_set);

void
mini_batch_gradient_descent(double *theta, double alpha, int steps, int batch_size, const DataSet *data_set);
