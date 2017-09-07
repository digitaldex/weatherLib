# weatherLib

Simple C Lib for Openweathermap. 
Its used to populate a SmartHome Display with Weather Data over Modbus TCP.

Dependencies:
 * [jansson](https://github.com/akheron/jansson)
 * [curl](https://github.com/curl/curl)
 * [libmodbus](https://github.com/stephane/libmodbus)
 * [API Key](https://openweathermap.org/api) from OpenweatherMap added in weather.h readCurl() function.
 
For General use have a look at main() from weather.c.

I would like to see [BACnet](https://sourceforge.net/projects/bacnet/) functionality within the Lib. 

Feel free to help
