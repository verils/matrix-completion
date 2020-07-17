#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE 16

typedef struct {
    char *date;
    int aqi;
} DailyAirData;

void read_air_data(FILE *fp, DailyAirData (*data)[], int skip, int data_size) {
    int buf_size = 255;
    char buf[buf_size];

    for (int i = 0; i < skip; ++i) {
        fgets(buf, buf_size, fp);
    }

    char *tok;
    int lines = 0;
    while (fgets(buf, buf_size, fp) && lines < data_size) {
        tok = strtok(buf, ",");
        int curr_col = 0;
        while (tok) {
            if (curr_col == 0) {
                (*data)[lines].date = strdup(tok);
            }
            if (curr_col == 2) {
                (*data)[lines].aqi = atoi(tok);
            }
            if (curr_col > 2) {
                break;
            }
            tok = strtok(NULL, ",\n");
            ++curr_col;
        }
        ++lines;
    }
}

void least_squares_linear_fitting(const int data[], int size, float *a, float *b) {
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

int exam_read_air_data() {
    char *filename = "air.csv";

    int data_size = DATA_SIZE;
    DailyAirData data[data_size];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return 1;
    }

    read_air_data(fp, &data, 1, data_size);

    fclose(fp);

    printf("AQI: \n");
    for (int i = 0; i < data_size; ++i) {
        printf("%s, %d\n", data[i].date, data[i].aqi);
    }
    printf("\n");

    int aqi_data[data_size];
    for (int i = 0; i < data_size; ++i) {
        aqi_data[i] = data[i].aqi;
    }

    float a, b;
    least_squares_linear_fitting(aqi_data, data_size, &a, &b);

    printf("a=%f, b=%f", a, b);

    return 0;
}
