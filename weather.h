#ifndef WEATHER_H
#define WEATHER_H

void updateWeather();


struct weather {
	const char *timestamp;   // %s	forecast timestamp
      	const char *icon;	 // %s	weather icon
	uint16_t id;	 		 // %i  weather description	
	uint16_t temp_min;          // %f  low temperature
	uint16_t temp_max;          // %f  high temperature
	uint16_t humidity;          // %f  relative humidity
	uint16_t temp;       	 // %f  current temperature
	uint16_t pressure;		 // %f	air pressure

};

size_t static curl_write(void *buffer, size_t size, size_t nmemb, void *userp) {
	userp += strlen(userp);
	memcpy(userp, buffer, nmemb);
	return nmemb;
}

void readCurl(char * result) {
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/forecast?q=Geldern,de&appid=YOURAPIKEY&units=metric");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

void parseJSON(int *size, struct weather *fetched_weather) {
	char *jsonData = (char *) malloc(20000);					//holds JSON Data
	json_t *root;
	json_error_t error;
	json_t *listArray, *weatherArray;
	json_t *main;
	json_t *elements, *innerelements;

	readCurl(jsonData);								//fetch Data from openweather
	root = json_loads(jsonData, 0, &error);
	free(jsonData);
	if(!root) {
		fprintf(stderr, "Error on line %d: %s\n", error.line, error.text);
		return;
	}

	json_t *temp, *pressure, *humidity, *temp_min, *temp_max, *timestamp, *id, *icon;

	listArray = json_object_get(root, "list");					//points on list array in JSON
	if(!json_is_array(listArray)){
		fprintf(stderr,"NG2\n");
	}

	for (int i = 0; i < json_array_size(listArray); i++) {				
		elements = json_array_get(listArray, i);	
		main = json_object_get(elements, "main");				//values from main object
			temp = json_object_get(main, "temp");
			pressure = json_object_get(main, "pressure");
			humidity = json_object_get(main, "humidity");
			temp_min = json_object_get(main, "temp_min");
			temp_max = json_object_get(main, "temp_max");

			fetched_weather[i].temp = json_number_value(temp) * 10;
			fetched_weather[i].pressure = json_number_value(pressure) * 10;
			fetched_weather[i].humidity = json_number_value(humidity) * 10;
			fetched_weather[i].temp_min = json_number_value(temp_min) * 10;
			fetched_weather[i].temp_max = json_number_value(temp_max) * 10;

		weatherArray = json_object_get(elements, "weather");			//values from weather array
		if(!json_is_array(weatherArray)){
			fprintf(stderr,"NG3\n");
		}
		for (int j = 0; j < json_array_size(weatherArray); j++) {
			innerelements = json_array_get(weatherArray, j);
			id = json_object_get(innerelements, "id");
			icon = json_object_get(innerelements, "icon");
			fetched_weather[i].icon = json_string_value(icon);
			fetched_weather[i].id = json_number_value(id);
		}
		timestamp = json_object_get(elements, "dt_txt");			//timestamp object
			fetched_weather[i].timestamp = json_string_value(timestamp);
	*size = i;	
	}
}

void writeModbus(int address, int index, struct weather *fetched_weather) {
	modbus_t *mb;
	int rc;
	mb = modbus_new_tcp("10.11.12.11", 1502);
	if (modbus_connect(mb) == -1) {
		fprintf(stderr, "Unable to connect: %s\n", modbus_strerror(errno));
		modbus_free(mb);
		exit(1);
	}

	rc = modbus_write_register(mb, address, fetched_weather[index].temp);
	rc = modbus_write_register(mb, address+10, fetched_weather[index].humidity);
	rc = modbus_write_register(mb, address+20, fetched_weather[index].pressure);
	rc = modbus_write_register(mb, address+30, fetched_weather[index].temp_min);
	rc = modbus_write_register(mb, address+40, fetched_weather[index].temp_max);
	rc = modbus_write_register(mb, address+50, fetched_weather[index].id);

	modbus_close(mb);
	modbus_free(mb);
}

void getDate(int offset, char *dateString) {
	time_t timer;
	struct tm *tm_info;
	int day;

	time(&timer);
	tm_info = localtime(&timer);
	day = tm_info->tm_mday;
	day = day + offset;
	tm_info->tm_mday = day;
	strftime(dateString, 11, "%Y-%m-%d", tm_info);
}

#endif
