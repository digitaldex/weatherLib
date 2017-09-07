#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <modbus.h>
#include <errno.h>
#include <time.h>
#include "weather.h"

struct weather fetched_weather[] = { 0 };

int main() {

	int size;
	int index;
	int address;
	char today[11];
	char day2[11]; 
	char day3[11];
	
	parseJSON(&size, fetched_weather);
	
	int offset = 0;
	getDate(offset, today);
	offset++;
	getDate(offset, day2);
	offset++;
	getDate(offset, day3);

	for(int i = 0; i <= size; i++) {
		if (strstr(fetched_weather[i].timestamp, today) && strstr(fetched_weather[i].timestamp, "09:00:00")) {
			writeModbus(0, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, today) && strstr(fetched_weather[i].timestamp, "12:00:00")) {
			writeModbus(100, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, today) && strstr(fetched_weather[i].timestamp, "18:00:00")) {
			writeModbus(200, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day2) && strstr(fetched_weather[i].timestamp, "09:00:00")) {
			writeModbus(300, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day2) && strstr(fetched_weather[i].timestamp, "12:00:00")) {
			writeModbus(400, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day2) && strstr(fetched_weather[i].timestamp, "18:00:00")) {
			writeModbus(500, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day3) && strstr(fetched_weather[i].timestamp, "09:00:00")) {
			writeModbus(600, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day3) && strstr(fetched_weather[i].timestamp, "12:00:00")) {
			writeModbus(700, i, fetched_weather);
		}
		if (strstr(fetched_weather[i].timestamp, day3) && strstr(fetched_weather[i].timestamp, "18:00:00")) {
			writeModbus(800, i, fetched_weather);
		}			
	}

	return 0;
}
