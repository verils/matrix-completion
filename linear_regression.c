
float hypothesis(float theta_0, float theta_1, float x) {
    return theta_0 + theta_1 * x;
}

float square_error(float theta_0, float theta_1, float x, float y) {
    float error = y - hypothesis(theta_0, theta_1, x);
    return error * error;
}

float mean_sum_of_squares(float theta_0, float theta_1, float x[], float y[], int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += square_error(theta_0, theta_1, x[i], y[i]);
    }
    return sum / (float) size;
}

void fit_normal_equation(const int days[], const int data[], int size, float *theta_0, float *theta_1) {
    float sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    for (int i = 0; i < size; ++i) {
        sum_x += (float) days[i];
        sum_y += (float) data[i];
        sum_xy += (float) days[i] * (float) data[i];
    }
    float size_f = (float) size;
    avg_x = sum_x / size_f;
    avg_y = sum_y / size_f;
    avg_xx = sum_xx / size_f;
    avg_xy = sum_xy / size_f;

    *theta_1 = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    *theta_0 = (sum_y - *theta_1 * sum_x) / size_f;
}

void fit_batch_gradient_descent(const int data[], int size, float *alpha, float *theta_0, float *theta_1) {

}
