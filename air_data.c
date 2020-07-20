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
            if (curr_col == 0) {
                data[lines].date = strdup(tok);
            }
            if (curr_col == 2) {
                data[lines].aqi = atoi(tok);
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
