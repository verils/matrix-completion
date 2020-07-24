
void least_squares_fitting(const int *data, int size, float *theta_0, float *theta_1) {
    float sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    for (int i = 0; i < size; ++i) {
        sum_x += (float) i;
        sum_y += (float) data[i];
        sum_xy += (float) i * (float) data[i];
    }
    float size_f = (float) size;
    avg_x = sum_x / size_f;
    avg_y = sum_y / size_f;
    avg_xx = sum_xx / size_f;
    avg_xy = sum_xy / size_f;

    *theta_1 = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    *theta_0 = (sum_y - *theta_1 * sum_x) / size_f;
}