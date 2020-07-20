
void least_squares_fitting(const int *data, int size, float *a, float *b) {
    float sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0, avg_x, avg_y, avg_xx, avg_xy;
    for (int i = 0; i < size; ++i) {
        sum_x += (float) i;
        sum_y += (float) data[i];
        sum_xy += (float) i * (float) data[i];
    }
    float f_size = (float) size;
    avg_x = sum_x / f_size;
    avg_y = sum_y / f_size;
    avg_xx = sum_xx / f_size;
    avg_xy = sum_xy / f_size;

    *b = (avg_xy - avg_x * avg_y) / (avg_xx - avg_x * avg_x);
    *a = (sum_y - *b * sum_x) / f_size;
}