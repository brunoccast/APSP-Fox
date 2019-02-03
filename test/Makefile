CC = mpic++
CFLAGS = -Wall --std=c++11

all: fox mem util

fox:
	$(CC) $(CFLAGS) fox.cpp ../src/fox.cpp ../src/util.cpp ../src/mem.cpp -o fox

mem:
	$(CC) $(CFLAGS) mem.cpp ../src/mem.cpp -o mem

util:
	$(CC) $(CFLAGS) util.cpp ../src/util.cpp ../src/mem.cpp -o util

clean:
	rm -f fox mem util
