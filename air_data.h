
struct DailyAirData {
    char *date;
    char *city;
    int aqi;
    int pm25;
    int pm10;
};

typedef struct DailyAirData DailyAirData;

void read_air_data_csv(FILE *, DailyAirData[], int);

int compare_air_date(const void *a, const void *b);

void print_air_data(DailyAirData [], int);
