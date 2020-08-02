
typedef struct {
    char *date;
    int get_day_of_20150101;
    char *city;
    int aqi;
    int pm25;
    int pm10;
} DailyAirData;

int air_data_read_csv(char *filename, DailyAirData data[], int max_size);

int air_data_copy_for_city(DailyAirData *source, DailyAirData *target, char *city, int max_size);

void air_data_sort(DailyAirData data[], int size);

void print_air_data(DailyAirData [], int);
