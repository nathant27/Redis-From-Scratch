TARGETS = server client

all: $(TARGETS)

server: server.cpp
	g++ -Wall -Wextra -O2 -g server.cpp -o server

client: client.cpp
	g++ -Wall -Wextra -O2 -g client.cpp -o client

clean:
	rm server
	rm client
