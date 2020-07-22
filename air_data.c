#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "air_data.h"

void read_air_data_csv(FILE *fp, DailyAirData data[], int size) {
    int buf_size = 255;
    char buf[buf_size];

    // Skip head line
    fgets(buf, buf_size, fp);

    char *tok;
    int lines = 0;
    while (fgets(buf, buf_size, fp) && lines < size) {
        tok = strtok(buf, ",");
        int curr_col = 0;
        while (tok) {
            if (curr_col > 4) {
                break;
            } else if (curr_col == 0) {
                data[lines].date = strdup(tok);
            } else if (curr_col == 1) {
                data[lines].city = strdup(tok);
            } else if (curr_col == 2) {
                data[lines].aqi = atoi(tok);
            } else if (curr_col == 3) {
                data[lines].pm25 = atoi(tok);
            } else if (curr_col == 4) {
                data[lines].pm10 = atoi(tok);
            }
            tok = strtok(NULL, ",\n");
            ++curr_col;
        }
        ++lines;
    }
}

int compare_air_date(const void *a, const void *b) {
    DailyAirData *a_data = (DailyAirData *) a, *b_data = (DailyAirData *) b;

    int cmp = strcmp(a_data->city, b_data->city);
    if (cmp != 0) {
        return cmp;
    }

    int a_year, a_month, a_day, b_year, b_month, b_day;
    sscanf(a_data->date, "%d/%d/%d", &a_year, &a_month, &a_day);
    sscanf(b_data->date, "%d/%d/%d", &b_year, &b_month, &b_day);

    if (a_year != b_year) {
        return a_year - b_year;
    }
    if (a_month != b_month) {
        return a_month - b_month;
    }
    if (a_day != b_day) {
        return a_day - b_day;
    }
    return 0;
}

void print_air_data(DailyAirData data[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s, %s, %d, %d, %d\n",
               data[i].date,
               data[i].city,
               data[i].aqi,
               data[i].pm25,
               data[i].pm10
        );
    }
}
