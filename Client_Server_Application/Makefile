# Portul pe care asculta serverul
PORT = 12345

# Adresa IP a serverului
IP_SERVER = 127.0.0.1

all: server subscriber

server: server.cpp
	g++ -Wall server.cpp -o server

subscriber: subscriber.cpp
	g++ -Wall subscriber.cpp -o subscriber

clean:
	rm -f server subscriber
