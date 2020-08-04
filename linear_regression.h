
typedef struct {
    double *feature;
    double *result;
    int size;
} DataSet;

void normal_equation(const DataSet *data_set, double *theta);

void batch_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps);

void stochastic_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps);

void mini_batch_gradient_descent(const DataSet *data_set, double *theta, double alpha, int steps, int batch_size);
