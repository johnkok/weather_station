#!/bin/bash
# Update Weather data on server

MY_SERVER="www.test.gr"

###########################################################

#Weather address: 7248
#Inside Temperature  31.70 C
#Outside Temperature 29.30 C
#Inside Humidity     32.00 %
#Ouside Humidity     41.00 %
#Wind Speed          5.80 Km/h
#Wind Direction      270.00
#Wind Gust           7.80 Km/h
#Atm. Pressure       1009.00 kPa
#Cum. Rain           0.00 mm
#Light intesity      105082.50 Lux

res=$(/home/user/./ws-weather)

# Update DB
wsr=`curl -X POST -H "User-Agent: Mozilla/5.0" -H "Accept: text/html" -H "Content-Type: text/html" --connect-timeout 3 ${MY_SERVER}/UpdateWeather.php?${res}`

echo $wsr


