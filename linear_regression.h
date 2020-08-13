
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

void normal_equation_train(double *theta, const DataSet *data_set);

double normal_equation_predict(double *theta, double feature);

void batch_gradient_descent_train(BatchGradientDescent *bgd, const DataSet *data_set);

double batch_gradient_descent_predict(BatchGradientDescent *bgd, double feature);

void stochastic_gradient_descent_train(StochasticGradientDescent *sgd, const DataSet *data_set);

double stochastic_gradient_descent_predict(StochasticGradientDescent *sgd, double feature);

void mini_batch_gradient_descent_train(MiniBatchGradientDescent *mbgd, const DataSet *data_set);

double mini_batch_gradient_descent_predict(MiniBatchGradientDescent *mbgd, double feature);
