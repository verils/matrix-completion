#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *const delim = ",";

void read_csv(FILE *fp, int (*data)[], int size, int col, int display) {
    char buf[255];
    char *tok;
    int lines = 0;
    while (fgets(buf, sizeof(buf), fp) && lines < size) {
        tok = strtok(buf, delim);
        int curr_col = 0;
        while (tok) {
            if (curr_col == col) {
                int num = atoi(tok);
                (*data)[lines] = num;
                if (display) {
                    printf("%s ", tok);
                }
                break;
            }
            tok = strtok(NULL, ",\n");
            ++curr_col;
        }
        if (display) {
            printf("\n");
        }
        ++lines;
    }
}

int exam_readfile_main() {
    char *filename = "air.csv";
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s", filename);
        return 1;
    }

    char buf[255];
    fgets(buf, sizeof(buf), fp);
    printf("%s", buf);

    int aqi_data_size = 16;
    int aqi_data[aqi_data_size];
    read_csv(fp, &aqi_data, aqi_data_size, 2, 1);
    printf("\n");

    printf("AQI: ");
    for (int i = 0; i < aqi_data_size; ++i) {
        printf("%d ", aqi_data[i]);
    }

    fclose(fp);
    return 0;
}
