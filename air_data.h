
struct DailyAirData {
    char *date;
    int aqi;
};

typedef struct DailyAirData DailyAirData;

void read_air_data_csv(FILE *, DailyAirData[], int);