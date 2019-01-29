.PHONY:all
all:tcp_server tcp_client
tcp_client:tcp_client.cc
	g++ -o $@ $^
tcp_server:tcp_server.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f tcp_server tcp_client
