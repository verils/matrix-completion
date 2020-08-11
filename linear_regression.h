
typedef struct {
    double *features;
    double *results;
    int size;
} DataSet;

typedef struct {
    double *theta;
    int dimension;
    double alpha;
    double expected_error;
    int steps;
} BatchGradientDescent;

typedef struct {
    double *theta;
    int dimension;
    double alpha;
    double expected_error;
    int steps;
} StochasticGradientDescent;

typedef struct {
    double *theta;
    int dimension;
    double alpha;
    double expected_error;
    int steps;
    int batch_size;
} MiniBatchGradientDescent;

double squares_error(const double *theta, const DataSet *data_set);

void train_normal_equation(double *theta, const DataSet *data_set);

double predict_normal_equation(double *theta, double feature);

void train_batch_gradient_descent(BatchGradientDescent *bgd, const DataSet *data_set);

double predict_batch_gradient_descent(BatchGradientDescent *bgd, double feature);

void train_stochastic_gradient_descent(StochasticGradientDescent *sgd, const DataSet *data_set);

double predict_stochastic_gradient_descent(StochasticGradientDescent *sgd, double feature);

void train_mini_batch_gradient_descent(MiniBatchGradientDescent *mbgd, const DataSet *data_set);

double predict_mini_batch_gradient_descent(MiniBatchGradientDescent *mbgd, double feature);
