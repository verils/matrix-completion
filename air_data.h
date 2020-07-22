
struct DailyAirData {
    char *date;
    char *city;
    int aqi;
    int pm25;
    int pm10;
};

typedef struct DailyAirData DailyAirData;

void read_air_data_csv(FILE *, DailyAirData[], int *, int);

int compare_air_date(const void *, const void *);

void print_air_data(DailyAirData [], int);
