#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "hidapi.h"
#include <time.h>
#include <unistd.h>

using namespace std;

float intTemp = 0;
float extTemp = 0;
float intHum = 0;
float extHum = 0;
float windSpeed = 0;
float windDir = 0;
float windGust = 0;
float pressure = 0;
float rain = 0;
float light = 0;
float uv = 0;

void parseWeatherData(unsigned char* buffer){

	if (buffer[15] & 0x40){
		printf("Error communicating with weather station\n");		
		return;
	}

	intHum = buffer[1];
	intTemp = (short int)(buffer[2] + buffer[3] * 256) / 10.0;
	extHum = buffer[4];
	extTemp = (short int)(buffer[5] + buffer[6] * 256) / 10.0;
	pressure = (buffer[7] + buffer[8] * 256) / 10.0;
	windSpeed = 3.6 * buffer[9] / 10.0;
	windGust = 3.6 * buffer[10] / 10.0;
	windDir = buffer[12] * 22.5;
	rain = buffer[13]* 0.33;
	if (buffer[16] != 0xFF && buffer[17] != 0xFF && buffer[18] != 0xFF)
		light = (buffer[16] + buffer[17] * 256 + buffer[18] * 65536) / 10.0;
	else
		light = 0;
	uv = buffer[19];

//	printf("Inside Temperature  %3.2f C\n"
//               "Outside Temperature %3.2f C\n"
//               "Inside Humidity     %3.2f %\n"
//               "Ouside Humidity     %3.2f %\n"
//               "Wind Speed          %3.2f Km/h\n"
//               "Wind Direction      %3.2f\n"
//               "Wind Gust           %3.2f Km/h\n"
//               "Atm. Pressure       %3.2f kPa\n"
//               "Cum. Rain           %3.2f mm\n"
//               "Light intesity      %3.2f Lux\n",
//		intTemp,extTemp,intHum,extHum,windSpeed,windDir,windGust,pressure,rain,light);
        printf("ws1=%3.2f&"
               "ws2=%3.2f&"
               "ws3=%3.2f&"
               "ws4=%3.2f&"
               "ws5=%3.2f&"
               "ws6=%3.2f&"
               "ws7=%3.2f&"
               "ws8=%3.2f&"
               "ws9=%3.2f&"
               "ws0=%3.2f",
                intTemp,extTemp,intHum,extHum,windSpeed,windDir,windGust,pressure,rain,light);

}

int readAddress(hid_device *handle, unsigned short int address, unsigned char* buffer){
int cnt = 0;

	buffer[0] = 0xA1;
	buffer[1] = address / 256;
	buffer[2] = (address % 256 );
	buffer[3] = 0x20;
	buffer[4] = 0xA1;
	buffer[5] = address / 256;
	buffer[6] = (address % 256 );
	buffer[7] = 0x20;
	buffer[8] = 0x00;

	if (hid_write(handle, buffer, 8) < 0){
		printf("Error writting hid\n");	
		return -1;
	}	

	cnt =+ hid_read_timeout(handle, &buffer[0], 8, 200);
	cnt =+ hid_read_timeout(handle, &buffer[8], 8, 200);
	cnt =+ hid_read_timeout(handle, &buffer[16], 8, 200);
	cnt =+ hid_read_timeout(handle, &buffer[24], 8, 200);
#ifdef DEBUG
	printf("%4.0d: %2.2X %2.2X %2.2X %2.2X - %2.2X %2.2X %2.2X %2.2X \n", 
		address, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
	printf("%4.0d: %2.2X %2.2X %2.2X %2.2X - %2.2X %2.2X %2.2X %2.2X \n", 
		address, buffer[8], buffer[9], buffer[10], buffer[11], buffer[12], buffer[13], buffer[14], buffer[15]);
	printf("%4.0d: %2.2X %2.2X %2.2X %2.2X - %2.2X %2.2X %2.2X %2.2X \n", 
		address, buffer[16], buffer[17], buffer[18], buffer[19], buffer[20], buffer[21], buffer[22], buffer[23]);
	printf("%4.0d: %2.2X %2.2X %2.2X %2.2X - %2.2X %2.2X %2.2X %2.2X \n", 
		address, buffer[24], buffer[25], buffer[26], buffer[27], buffer[28], buffer[29], buffer[30], buffer[31]);
#endif
	return cnt;
}

int main(int argc, char *argv[]){

hid_device *handle;
unsigned char buffer[64];
unsigned int offset, updCount, lastAddress;
int i;

	updCount = 0;

	handle = hid_open(0x1941, 0x8021, NULL);
	if (handle <= 0){
		printf("Error opening HID device\n");
		hid_close(handle);
		return 1;
	}

	hid_set_nonblocking(handle, 0);

	if (readAddress(handle, 0, buffer) < 0){
		hid_close(handle);
		printf("Error reading HID device\n");
		return 1;
	}
	
	if (buffer[0] != 0x55 || buffer[1] != 0xAA || buffer[2] != 0xFF || buffer[3] != 0xFF){
		printf("Error reading data header %X %X %X %X\n", 
                        buffer[0], buffer[1], buffer[2], buffer[3]);
		return 1;
	}
		
	lastAddress = buffer[31] * 256 + buffer[30];
//	printf ("Weather address: %X\n", lastAddress);

	readAddress(handle, lastAddress, buffer);

	parseWeatherData(buffer);

	hid_close(handle);

	return 0;

}


