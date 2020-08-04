#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "air_data.h"

int get_day_of_unix_epoch(char *date) {
    int year, month, day;
    sscanf(date, "%d/%d/%d", &year, &month, &day);
    struct tm time_data = {0, 0, 0, day, month - 1, year - 1900};
    time_t seconds = mktime(&time_data);
    int days = (int) (seconds / 60 / 60 / 24);
    return days;
}

int air_data_read_csv(char *filename, DailyAirData data[], int max_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s", filename);
        exit(EXIT_FAILURE);
    }

    int buf_size = 255;
    char buf[buf_size];

    // Skip head line
    fgets(buf, buf_size, file);

    char *tok;
    int lines = 0;
    while (fgets(buf, buf_size, file) && lines < max_size) {
        tok = strtok(buf, ",");
        int curr_col = 0;
        while (tok) {
            if (curr_col > 4) {
                break;
            } else if (curr_col == 0) {
                char *date = strdup(tok);
                data[lines].date = date;
                data[lines].day_of_unix_epoch = get_day_of_unix_epoch(date);
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

    fclose(file);

    return lines;
}

int air_data_copy_for_city(DailyAirData *source, DailyAirData *target, char *city, int max_size) {
    int count = 0;
    for (int i = 0; i < max_size; ++i) {
        DailyAirData data = source[i];
        int result = strcmp(data.city, city);
        if (result == 0) {
            target[count++] = data;
        }
    }
    return count;
}

int air_data_compare(const void *a, const void *b) {
    DailyAirData *a_data = (DailyAirData *) a, *b_data = (DailyAirData *) b;

    int cmp = strcmp(a_data->city, b_data->city);
    if (cmp != 0) {
        return cmp;
    }

    return a_data->day_of_unix_epoch - b_data->day_of_unix_epoch;
}

void air_data_sort(DailyAirData data[], int size) {
    qsort(data, size, sizeof(DailyAirData), air_data_compare);
}

void print_air_data(DailyAirData data[], int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s, %d, %s, %d, %d, %d\n",
               data[i].date,
               data[i].day_of_unix_epoch,
               data[i].city,
               data[i].aqi,
               data[i].pm25,
               data[i].pm10
        );
    }
}
