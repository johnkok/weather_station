HEADERS = 

OBJECTS = \
	hid.o \
        ws-weather.o

CXXFLAGS = -O3 -o prog -g3 -rdynamic -D_GNU_SOURCE \
	-Wno-write-strings -Wno-unused-result \
	-L/usr/local/lib -L/lib 

LIBS = 	-lpthread -lcurl \
	-lstdc++ \
	-ludev -lm


default: ws-weather 

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

ws-weather: $(OBJECTS)
	gcc $(OBJECTS) $(LIBS) $(CXXFLAGS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f ws-weather

